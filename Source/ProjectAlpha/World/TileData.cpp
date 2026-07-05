// Fill out your copyright notice in the Description page of Project Settings.


#include "TileData.h"

FTileData::FTileData(FVector location, bool isStatic, bool isSpawnPoint)
{
    m_location = location;
    m_isStatic = isStatic;
    m_isSpawnPoint = isSpawnPoint;
}
