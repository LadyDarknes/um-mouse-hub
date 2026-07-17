#pragma once


class IMosueMethod{ // I realise this was wrong spelling, but I will keep it I kinda like that lol
    public:
    virtual ~IMosueMethod() = default;

    virtual bool move_to(int x, int y) = 0;
    virtual bool move_relative(int dx, int dy) = 0;
};