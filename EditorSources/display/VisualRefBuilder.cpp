//
// Created by Roman on 12/4/2025.
//

#include "VisualRefBuilder.h"

#include "VisualRef.h"
#include "Core/ParserUtils.h"

nsVisualObject2d * nsVisualRefBuilder::CreateDefault() {
    return new nsVisualRef();
}

bool nsVisualRefBuilder::Parse(script_state_t *ss, nsVisualObject2d *object, nsVisualCreationContext2d *context) {
    if (!nsVisualBuilder2d::Parse(ss, object, context)) {
        return false;
    }

    const auto ref = Cast<nsVisualRef>(object);
    if (!ref) {
        return false;
    }
    ref->_context = context;

    ref->source = ParseStrP(ss, "source");

    if (ref->_ref) {

    }

    return true;
}
