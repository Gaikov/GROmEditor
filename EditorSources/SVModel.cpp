//
// Created by Roman on 10/22/2024.
//

#include "SVModel.h"
#include "nsLib/log.h"

nsSVModel::nsSVModel() : zoom(1), xFlip(false), yFlip(false) {

}

void nsSVModel::OnCreated() {
    Log::Info("sv model created");
}

