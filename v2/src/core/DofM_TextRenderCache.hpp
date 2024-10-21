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
constexpr uint8_t b1 = 8 * 7;
constexpr uint8_t b2 = 8 * 6;
constexpr uint8_t b3 = 8 * 5;
constexpr uint8_t b4 = 8 * 4;
constexpr uint8_t b5 = 8 * 3;
constexpr uint8_t b6 = 8 * 2;
constexpr uint8_t b7 = 8 * 1;
constexpr uint8_t b8 = 0;

typedef uint64_t CacheKeyType;

class DofM_TextRenderCache
{
private:
    class RenderCacheObject
    {
    private:
        SDL_Texture *Texture;
        std::string BackendChar;
        TTF_Font *fontcontext;
        SDL_Renderer *renderercontext;
        SDL_Color color;
        bool FreeTextureOnDeconstruct = false;

    public:
        std::chrono::high_resolution_clock::time_point LastGet = std::chrono::high_resolution_clock::now();
        RenderCacheObject(RenderCacheObject const& obj) = delete;
        RenderCacheObject(RenderCacheObject&& obj)
        {
            //std::cout << "Moving Texture cache object" << std::endl;
            this->BackendChar = obj.BackendChar;
            this->fontcontext = obj.fontcontext;
            this->renderercontext = obj.renderercontext;
            this->Texture = obj.Texture;
            obj.FreeTextureOnDeconstruct = false;
        }

        RenderCacheObject(const std::string &letter, TTF_Font *font, SDL_Renderer *renderer, SDL_Color fgcolor)
        {
            //std::cout << "Creating Texture cache object" << std::endl;
            this->BackendChar = letter;
            this->fontcontext = font;
            this->renderercontext = renderer;
            //std::cout << "Creating Texture cache object" << std::endl;
            SDL_Surface *text_surface = TTF_RenderUTF8_Blended(this->fontcontext, this->BackendChar.c_str(), fgcolor);
            this->Texture = SDL_CreateTextureFromSurface(this->renderercontext, text_surface);
            SDL_FreeSurface(text_surface);
            this->color = fgcolor;
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

    std::unordered_map< CacheKeyType, RenderCacheObject > RenderCache;
    void AddToCache(const std::string &letter, TTF_Font *font, SDL_Renderer *renderer, const SDL_Color fgcolor)
    {
        auto key = CreateKey(letter,fgcolor);
        if (letter == "0")
        {
        fmt::print("Making cache on key: {} ({})\n", letter, key);
        }
        RenderCache.emplace (key , RenderCacheObject(letter, font, renderer, fgcolor) );
    }

    /*
     *   b1       b2       b3       b4       b5       b6       b7       b8
     *00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
     *
    */
    uint64_t CreateKey(const std::string &letter, const SDL_Color fgcolor)
    {
        uint64_t rval = 0ULL;
        switch (letter.size())
        {
        case 0:
            break;
        case 1:
            rval |= (uint64_t)letter.at(0) << b1;
            break;
        case 2:
            rval |= (uint64_t)letter.at(0) << b1;
            rval |= (uint64_t)letter.at(1) << b2;
            break;
        case 3:
            rval |= (uint64_t)letter.at(0) << b1;
            rval |= (uint64_t)letter.at(1) << b2;
            rval |= (uint64_t)letter.at(2) << b3;
            break;
        case 4:
            rval |= (uint64_t)letter.at(0) << b1;
            rval |= (uint64_t)letter.at(1) << b2;
            rval |= (uint64_t)letter.at(2) << b3;
            rval |= (uint64_t)letter.at(3) << b4;
            break;
        }
        rval |= (uint64_t)fgcolor.a << b5;
        rval |= (uint64_t)fgcolor.b << b6;
        rval |= (uint64_t)fgcolor.g << b7;
        rval |= (uint64_t)fgcolor.r << b8;
        return rval;
    }


public:
    std::chrono::high_resolution_clock::time_point LastCacheRefresh = std::chrono::high_resolution_clock::now();


    const float TextureMaxAgeMs = 2.0f;
    SDL_Texture* GetTexture(const std::string &letter, TTF_Font *font, SDL_Renderer *renderer, SDL_Color fgcolor)
    {
        SDL_Texture* rval;

        if (IsCacheCleanDue())
        {
            CleanCache();
        }

        auto lookupkey = CreateKey( letter, fgcolor);
        if (RenderCache.find(lookupkey) == RenderCache.end())
        {
            //std::cout << "Adding Texture cache" << std::endl;
            AddToCache( letter, font, renderer, fgcolor);
        }
        rval = RenderCache.at(lookupkey).GetTexture();


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
        std::vector<CacheKeyType> deletelist;
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


#endif //DUNGONSOFMORGINAR_DOFM_TEXTRENDERCACHE_H
