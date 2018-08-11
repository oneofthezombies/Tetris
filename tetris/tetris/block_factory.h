#pragma once

#include "utility.h"

#include "block.h"

struct BlockFactory 
{
    enum class Shape
    {
        kIdle,
        kRedS,
        kRedL,
        kBlueS,
        kBlueL,
        kM,
        kI,
        kT
    };

    Block* operator()(Shape shape, tag_t tag = tag_t::kNext)
    {
        std::vector<Vector2<>> positions;
        switch (shape)
        {
        case Shape::kRedS:
        {
            positions.emplace_back(Vector2<>(-1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 1.0f));
            positions.emplace_back(Vector2<>(1.0f, 1.0f));
        }
        break;
        case Shape::kRedL:
        {
            positions.emplace_back(Vector2<>(-1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 0.0f));
            positions.emplace_back(Vector2<>(-1.0f, 1.0f));
        }
        break;
        case Shape::kBlueS:
        {
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 0.0f));
            positions.emplace_back(Vector2<>(-1.0f, 1.0f));
            positions.emplace_back(Vector2<>(0.0f, 1.0f));
        }
        break;
        case Shape::kBlueL:
        {
            positions.emplace_back(Vector2<>(-1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 1.0f));
        }
        break;
        case Shape::kM:
        {
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 1.0f));
            positions.emplace_back(Vector2<>(1.0f, 1.0f));
        }
        break;
        case Shape::kT:
        {
            positions.emplace_back(Vector2<>(-1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 1.0f));
        }
        break;
        default:    // Shape::kI
        {
            positions.emplace_back(Vector2<>(-1.0f, 0.0f));
            positions.emplace_back(Vector2<>(0.0f, 0.0f));
            positions.emplace_back(Vector2<>(1.0f, 0.0f));
            positions.emplace_back(Vector2<>(2.0f, 0.0f));
        }
        break;
        }

        COLORREF color;
        switch (shape)
        {
        case Shape::kRedS:
            color = Color::kRed;
        break;
        case Shape::kRedL:
            color = Color::kCyan;
        break;
        case Shape::kBlueS:
            color = Color::kBlue;
        break;
        case Shape::kBlueL:
            color = Color::kYellow;
        break;
        case Shape::kM:
            color = Color::kMagenta;
        break;
        case Shape::kI:
            color = Color::kGreen;
        break;
        case Shape::kT:
            color = Color::kGray;
        break;
        }

        Block* block = new Block(tag, Vector2<>(13.0f, 3.0f));
        for (auto position : positions)
            block->setChild(new Piece(tag_t::kPiece, position, color));

        return block;
    }
};