//
// Created by eh on 16.08.24.
//

#ifndef DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H
#define DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H

#include <unordered_map>
#include <string>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class DofM_TextRenderCache
{
private:
    class RenderCacheObject
    {
    private:
        SDL_Texture *Texture;
        std::string BackendString;
        TTF_Font *fontcontext;
        SDL_Renderer *renderercontext;
        bool FreeTextureOnDeconstruct = false;

    public:
        std::chrono::high_resolution_clock::time_point LastGet = std::chrono::high_resolution_clock::now();
        RenderCacheObject(RenderCacheObject const& obj) = delete;
        RenderCacheObject(RenderCacheObject&& obj)
        {
            //std::cout << "Moving Texture cache object" << std::endl;
            this->BackendString = obj.BackendString;
            this->fontcontext = obj.fontcontext;
            this->renderercontext = obj.renderercontext;
            this->Texture = obj.Texture;
            obj.FreeTextureOnDeconstruct = false;
        }

        RenderCacheObject(std::string &text, TTF_Font *font, SDL_Renderer *renderer, SDL_Color fgcolor)
        {
            //std::cout << "Creating Texture cache object" << std::endl;
            this->BackendString = text;
            this->fontcontext = font;
            this->renderercontext = renderer;
            //std::cout << "Creating Texture cache object" << std::endl;
            SDL_Surface *text_surface = TTF_RenderText_Solid(this->fontcontext, this->BackendString.c_str(), fgcolor);
            this->Texture = SDL_CreateTextureFromSurface(this->renderercontext, text_surface);
            SDL_FreeSurface(text_surface);
        }
        SDL_Texture* GetTexture()
        {
            LastGet = std::chrono::high_resolution_clock::now();
            return this->Texture;
        }
        float GetAgeInMs()
        {
            std::chrono::duration<float> duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->LastGet);
            return duration.count();
        }
        ~RenderCacheObject()
        {
            if (FreeTextureOnDeconstruct)
            {
                //std::cout << "Destroying Texture" << std::endl;
                SDL_DestroyTexture(Texture);
            }
            //std::cout << "Deconstruct Texture cache object" << std::endl;
        }

    };
    void AddToCache(std::string text, TTF_Font *font, SDL_Renderer *renderer, SDL_Color fgcolor)
    {
        RenderCache.emplace ( text, RenderCacheObject(text, font, renderer, fgcolor) );
    }
public:
    std::chrono::high_resolution_clock::time_point LastCacheRefresh = std::chrono::high_resolution_clock::now();
    std::unordered_map<std::string, RenderCacheObject> RenderCache;
    const float TextureMaxAgeMs = 2.0f;
    std::vector<SDL_Texture*> GetTexture(std::string text, TTF_Font *font, SDL_Renderer *renderer, SDL_Color fgcolor)
    {
        std::vector<SDL_Texture*> rval;
        std::chrono::duration<float> duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->LastCacheRefresh);
        //std::cout << "Debug: GetTexture is called "<< duration.count()<< std::endl;
        if (duration.count() > 10.0f)
        {
            std::vector<std::string> deletelist;
            for (auto& x: this->RenderCache)
            {
                //std::cout << "Debug: Age: " << x.second.GetAgeInMs() << std::endl;
                if (x.second.GetAgeInMs() > TextureMaxAgeMs)
                {
                    deletelist.push_back(x.first);
                }
            }
            for (auto &x: deletelist)
            {
                this->RenderCache.erase(x);
            }
            LastCacheRefresh = std::chrono::high_resolution_clock::now();
            //std::cout << "Removed " << deletelist.size() << " from texture cache" << std::endl;
        }
        for (auto &letter:text)
        {
            if (RenderCache.find(std::string(1, letter)) == RenderCache.end())
            {
                //std::cout << "Adding Texture cache" << std::endl;
                AddToCache(text, font, renderer, fgcolor);
            }
            rval.push_back(RenderCache.at(text).GetTexture());
        }

        return rval;
    }

};


#endif //DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H