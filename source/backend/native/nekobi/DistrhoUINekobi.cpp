/*
 * DISTRHO Nekobi Plugin, based on Nekobee by Sean Bolton and others.
 * Copyright (C) 2013 Filipe Coelho <falktx@falktx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the GPL.txt file
 */

#include "DistrhoUINekobi.hpp"

#include "dgl/ImageAboutWindow.hpp"

START_NAMESPACE_DISTRHO

// -------------------------------------------------

DistrhoUINekobi::DistrhoUINekobi()
    : OpenGLUI()
{
    Window* win = getParent();

    fNeko.setTimerSpeed(4);

    // background
    fImgBackground = Image(DistrhoArtworkNekobi::backgroundData, DistrhoArtworkNekobi::backgroundWidth, DistrhoArtworkNekobi::backgroundHeight, GL_BGR);

    // slider
    Image sliderImage(DistrhoArtworkNekobi::sliderData, DistrhoArtworkNekobi::sliderWidth, DistrhoArtworkNekobi::sliderHeight);

    fSliderWaveform = new ImageSlider(win, sliderImage);
    fSliderWaveform->setStartPos(135, 39);
    fSliderWaveform->setEndPos(135, 65);
    fSliderWaveform->setRange(0.0f, 1.0f);
    fSliderWaveform->setValue(0.0f);
    fSliderWaveform->setCallback(this);

    // knobs
    Image knobImage(DistrhoArtworkNekobi::knobData, DistrhoArtworkNekobi::knobWidth, DistrhoArtworkNekobi::knobHeight);

    // knob Tuning
    fKnobTuning = new ImageKnob(win, knobImage);
    fKnobTuning->setPos(44, 46);
    fKnobTuning->setRange(-12.0f, 12.0f);
    fKnobTuning->setValue(0.0f);
    fKnobTuning->setCallback(this);

    // knob Cutoff
    fKnobCutoff = new ImageKnob(win, knobImage);
    fKnobCutoff->setPos(187, 46);
    fKnobCutoff->setRange(0.0f, 100.0f);
    fKnobCutoff->setValue(25.0f);
    fKnobCutoff->setCallback(this);

    // knob Resonance
    fKnobResonance = new ImageKnob(win, knobImage);
    fKnobResonance->setPos(260, 46);
    fKnobResonance->setRange(0.0f, 95.0f);
    fKnobResonance->setValue(25.0f);
    fKnobResonance->setCallback(this);

    // knob Env Mod
    fKnobEnvMod = new ImageKnob(win, knobImage);
    fKnobEnvMod->setPos(332, 46);
    fKnobEnvMod->setRange(0.0f, 100.0f);
    fKnobEnvMod->setValue(50.0f);
    fKnobEnvMod->setCallback(this);

    // knob Decay
    fKnobDecay = new ImageKnob(win, knobImage);
    fKnobDecay->setPos(404, 46);
    fKnobDecay->setRange(0.0f, 100.0f);
    fKnobDecay->setValue(75.0f);
    fKnobDecay->setCallback(this);

    // knob Accent
    fKnobAccent = new ImageKnob(win, knobImage);
    fKnobAccent->setPos(476, 46);
    fKnobAccent->setRange(0.0f, 100.0f);
    fKnobAccent->setValue(25.0f);
    fKnobAccent->setCallback(this);

    // knob Volume
    fKnobVolume = new ImageKnob(win, knobImage);
    fKnobVolume->setPos(548, 46);
    fKnobVolume->setRange(0.0f, 100.0f);
    fKnobVolume->setValue(75.0f);
    fKnobVolume->setCallback(this);
}

DistrhoUINekobi::~DistrhoUINekobi()
{
    delete fSliderWaveform;
    delete fKnobTuning;
    delete fKnobCutoff;
    delete fKnobResonance;
    delete fKnobEnvMod;
    delete fKnobDecay;
    delete fKnobAccent;
    delete fKnobVolume;
}

// -------------------------------------------------
// DSP Callbacks

void DistrhoUINekobi::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case DistrhoPluginNekobi::paramTuning:
        fKnobTuning->setValue(value);
        break;
    case DistrhoPluginNekobi::paramWaveform:
        fSliderWaveform->setValue(value);
        break;
    case DistrhoPluginNekobi::paramCutoff:
        fKnobCutoff->setValue(value);
        break;
    case DistrhoPluginNekobi::paramResonance:
        fKnobResonance->setValue(value);
        break;
    case DistrhoPluginNekobi::paramEnvMod:
        fKnobEnvMod->setValue(value);
        break;
    case DistrhoPluginNekobi::paramDecay:
        fKnobDecay->setValue(value);
        break;
    case DistrhoPluginNekobi::paramAccent:
        fKnobAccent->setValue(value);
        break;
    case DistrhoPluginNekobi::paramVolume:
        fKnobVolume->setValue(value);
        break;
    }
}

void DistrhoUINekobi::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fSliderWaveform->setValue(0.0f);
    fKnobTuning->setValue(0.0f);
    fKnobCutoff->setValue(25.0f);
    fKnobResonance->setValue(25.0f);
    fKnobEnvMod->setValue(50.0f);
    fKnobDecay->setValue(75.0f);
    fKnobAccent->setValue(25.0f);
    fKnobVolume->setValue(50.0f);
}

void DistrhoUINekobi::d_noteReceived(bool onOff, uint8_t, uint8_t note, uint8_t)
{
    return;

    (void)onOff;
    (void)note;
}

// ---------------------------------------------
// UI Callbacks

void DistrhoUINekobi::d_uiIdle()
{
    if (fNeko.idle())
        repaint();
}

// -------------------------------------------------
// Widget Callbacks

void DistrhoUINekobi::imageButtonClicked(ImageButton* button, int)
{
    (void)button;
}

void DistrhoUINekobi::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobTuning)
        d_editParameter(DistrhoPluginNekobi::paramTuning, true);
    else if (knob == fKnobCutoff)
        d_editParameter(DistrhoPluginNekobi::paramCutoff, true);
    else if (knob == fKnobResonance)
        d_editParameter(DistrhoPluginNekobi::paramResonance, true);
    else if (knob == fKnobEnvMod)
        d_editParameter(DistrhoPluginNekobi::paramEnvMod, true);
    else if (knob == fKnobDecay)
        d_editParameter(DistrhoPluginNekobi::paramDecay, true);
    else if (knob == fKnobAccent)
        d_editParameter(DistrhoPluginNekobi::paramAccent, true);
    else if (knob == fKnobVolume)
        d_editParameter(DistrhoPluginNekobi::paramVolume, true);
}

void DistrhoUINekobi::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobTuning)
        d_editParameter(DistrhoPluginNekobi::paramTuning, false);
    else if (knob == fKnobCutoff)
        d_editParameter(DistrhoPluginNekobi::paramCutoff, false);
    else if (knob == fKnobResonance)
        d_editParameter(DistrhoPluginNekobi::paramResonance, false);
    else if (knob == fKnobEnvMod)
        d_editParameter(DistrhoPluginNekobi::paramEnvMod, false);
    else if (knob == fKnobDecay)
        d_editParameter(DistrhoPluginNekobi::paramDecay, false);
    else if (knob == fKnobAccent)
        d_editParameter(DistrhoPluginNekobi::paramAccent, false);
    else if (knob == fKnobVolume)
        d_editParameter(DistrhoPluginNekobi::paramVolume, false);
}

void DistrhoUINekobi::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobTuning)
        d_setParameterValue(DistrhoPluginNekobi::paramTuning, value);
    else if (knob == fKnobCutoff)
        d_setParameterValue(DistrhoPluginNekobi::paramCutoff, value);
    else if (knob == fKnobResonance)
        d_setParameterValue(DistrhoPluginNekobi::paramResonance, value);
    else if (knob == fKnobEnvMod)
        d_setParameterValue(DistrhoPluginNekobi::paramEnvMod, value);
    else if (knob == fKnobDecay)
        d_setParameterValue(DistrhoPluginNekobi::paramDecay, value);
    else if (knob == fKnobAccent)
        d_setParameterValue(DistrhoPluginNekobi::paramAccent, value);
    else if (knob == fKnobVolume)
        d_setParameterValue(DistrhoPluginNekobi::paramVolume, value);
}

void DistrhoUINekobi::imageSliderDragStarted(ImageSlider* slider)
{
    if (slider != fSliderWaveform)
        return;

    d_editParameter(DistrhoPluginNekobi::paramWaveform, true);
}

void DistrhoUINekobi::imageSliderDragFinished(ImageSlider* slider)
{
    if (slider != fSliderWaveform)
        return;

    d_editParameter(DistrhoPluginNekobi::paramWaveform, false);
}

void DistrhoUINekobi::imageSliderValueChanged(ImageSlider* slider, float value)
{
    if (slider != fSliderWaveform)
        return;

    d_setParameterValue(DistrhoPluginNekobi::paramWaveform, value);
}

void DistrhoUINekobi::onDisplay()
{
    fImgBackground.draw();
    fNeko.draw();
}

// -------------------------------------------------

UI* createUI()
{
    return new DistrhoUINekobi();
}

// -------------------------------------------------

END_NAMESPACE_DISTRHO
