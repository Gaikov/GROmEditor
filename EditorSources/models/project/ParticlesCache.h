// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesModel.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ProjectSubModel.h"
#include "Engine/renderer/particles/factory/ParticlesFactory.h"
#include "Engine/renderer/particles/factory/ParticlesLoader.h"

class nsParticlesCache final : public nsProjectSubModel, public nsParticlesLoader {
public:
    ~nsParticlesCache() override;
    nsParticlesBehaviour * LoadParticles(const char *fileName) override;
    const char * GetParticlesPath(nsParticlesBehaviour *particles) override;

protected:
    void Reset() override;
    bool Load(const nsFilePath &folder) override;
    bool Save(const nsFilePath &folder) override;

private:
    std::map<std::string, nsParticlesBehaviour *> _cache;
    nsParticlesFactory _factory;
};
