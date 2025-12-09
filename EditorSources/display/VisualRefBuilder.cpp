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
    ref->source = context->ParseAssetPath(ss, "source").AsChar();

    if (ref->_ref) {
        const auto item = ref->_ref;
        const auto o = ref;

        ref->id = ParseStrP(ss, "id", o->id.c_str());

        if (!ParseAnchor(ss, "xMin", o->xMin)) {
            o->xMin = item->xMin;
        }
        if (!ParseAnchor(ss, "yMin", o->yMin)) {
            o->yMin = item->yMin;
        }
        if (!ParseAnchor(ss, "xMax", o->xMax)) {
            o->xMax = item->xMax;
        }
        if (!ParseAnchor(ss, "yMax", o->yMax)) {
            o->yMax = item->yMax;
        }
        if (!ParseAnchor(ss, "xCenter", o->xCenter)) {
            o->xCenter = item->xCenter;
        }
        if (!ParseAnchor(ss, "yCenter", o->yCenter)) {
            o->yCenter = item->yCenter;
        }

        o->origin.pos = {
            ParseFloat(ss, "x", o->_itemPos.x),
            ParseFloat(ss, "y", o->_itemPos.y)};

        o->origin.scale = {
            ParseFloat(ss, "sx", o->_itemScale.x),
            ParseFloat(ss, "sy", o->_itemScale.y)};

        if (ps_var_begin(ss, "angle")) {
            o->origin.angle = nsMath::ToRad(ps_var_f(ss));
        } else {
            o->origin.angle = o->_itemAngle;
        }

        o->visible = ParseBool(ss, "visible", o->_itemVisible);
    }

    return true;
}

bool nsVisualRefBuilder::SerializeProps(nsScriptSaver &saver, nsVisualObject2d *o, nsVisualCreationContext2d *context) {

    const auto ref = Cast<nsVisualRef>(o);
    ref->UpdateRef(); //update if item was changed
    const auto item = ref->_ref;

    if (!ref->source->empty()) {
        saver.VarString("source", ref->source->c_str());
    }

    if (!item) {
        nsVisualBuilder2d::SerializeProps(saver, o, context);
    } else {
        if (o->id != item->id) {
            saver.VarString("id", o->id.c_str());
        }

        if (o->xMin != item->xMin) {
            SaveAnchor(saver, "xMin", o->xMin);
        }
        if (o->yMin != item->yMin) {
            SaveAnchor(saver, "yMin", o->yMin);
        }
        if (o->xMax != item->xMax) {
            SaveAnchor(saver, "xMax", o->xMax);
        }
        if (o->yMax != item->yMax) {
            SaveAnchor(saver, "yMax", o->yMax);
        }
        if (o->xCenter != item->xCenter) {
            SaveAnchor(saver, "xCenter", o->xCenter);
        }
        if (o->yCenter != item->yCenter) {
            SaveAnchor(saver, "yCenter", o->yCenter);
        }

        saver.VarFloat("x", o->origin.pos->x, ref->_itemPos.x);
        saver.VarFloat("y", o->origin.pos->y, ref->_itemPos.y);
        saver.VarFloat("sx", o->origin.scale->x, ref->_itemScale.x);
        saver.VarFloat("sy", o->origin.scale->y, ref->_itemScale.y);
        saver.VarFloat("angle",  nsMath::ToDeg(o->origin.angle), nsMath::ToDeg(ref->_itemAngle));
        saver.VarBool("visible", o->visible, ref->_itemVisible);

    }

    return true;
}