#pragma once


class IMosueMethod{
    public:
    virtual ~IMosueMethod() = default;

    virtual bool move_to(int x, int y) = 0;
    virtual bool move_relative(int dx, int dy) = 0;
};