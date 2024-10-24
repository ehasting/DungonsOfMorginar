//
// Created by eh on 16.08.24.
//

#ifndef DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H
#define DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "fmt/xchar.h"
#include "core/screencharacter.hpp"

namespace DofM
{
class DofM_TextRenderCache
{
private:
    class RenderCacheObject
    {
    private:
        SDL_Texture *Texture;
        ScreenCharacter ScreenChar;
        TTF_Font *fontcontext;
        SDL_Renderer *renderercontext;
        bool FreeTextureOnDeconstruct = false;

    public:
        std::chrono::high_resolution_clock::time_point LastGet = std::chrono::high_resolution_clock::now();
        RenderCacheObject(RenderCacheObject const& obj) = delete;
        RenderCacheObject(RenderCacheObject&& obj)
        {
            //std::cout << "Moving Texture cache object" << std::endl;
            this->ScreenChar = obj.ScreenChar;
            this->fontcontext = obj.fontcontext;
            this->renderercontext = obj.renderercontext;
            this->Texture = obj.Texture;
            obj.FreeTextureOnDeconstruct = false;
        }

        RenderCacheObject(ScreenCharacter &sc, TTF_Font *font, SDL_Renderer *renderer)
        {
            //std::cout << "Creating Texture cache object" << std::endl;
            this->ScreenChar = sc;
            this->fontcontext = font;
            this->renderercontext = renderer;
            //std::cout << "Creating Texture cache object" << std::endl;
            SDL_Surface *text_surface = TTF_RenderUTF8_Blended(this->fontcontext, this->ScreenChar.Character.c_str(), this->ScreenChar.Color);
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

    std::unordered_map< uint64_t, RenderCacheObject > RenderCache;
    void AddToCache(ScreenCharacter &sc, TTF_Font *font, SDL_Renderer *renderer)
    {
        RenderCache.emplace ( sc.CreateKey() , RenderCacheObject(sc, font, renderer) );
    }

    /*
     *   b1       b2       b3       b4       b5       b6       b7       b8
     *00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
     *
    */



public:
    std::chrono::high_resolution_clock::time_point LastCacheRefresh = std::chrono::high_resolution_clock::now();


    const float TextureMaxAgeMs = 2.0f;
    SDL_Texture* GetTexture(ScreenCharacter &sc, TTF_Font *font, SDL_Renderer *renderer)
    {
        SDL_Texture* rval;

        if (IsCacheCleanDue())
        {
            CleanCache();
        }

        if (RenderCache.find(sc.CreateKey()) == RenderCache.end())
        {
            //std::cout << "Adding Texture cache" << std::endl;
            AddToCache( sc, font, renderer );
        }
        rval = RenderCache.at(sc.CreateKey()).GetTexture();

        return rval;
    }

    bool IsCacheCleanDue()
    {
        return false; // DEBUG - not sure if we even need to clean cache.
        std::chrono::duration<float> duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->LastCacheRefresh);
        if (duration.count() > 100000.0f)
        {
            LastCacheRefresh = std::chrono::high_resolution_clock::now();
            return true;
        }
        return false;
    }

    void CleanCache()
    {
        //std::vector<uint64_t> deletelist;
        std::vector<uint64_t> deletelist;
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
    }

};

}
#endif //DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H
