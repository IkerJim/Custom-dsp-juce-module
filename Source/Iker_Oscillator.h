/*
  ==============================================================================

    Iker_Oscillator.h
    Created: 23 Apr 2026 10:15:17pm
    Author:  ikerr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Iker_dsp
{
    template <typename SampleType>
    class Oscillator
    {
    public:
        void prepare(const juce::dsp::ProcessSpec& spec)
        {
            m_frequency = 1;
            m_time = 0;
            m_timeDeviation = 1 / spec.sampleRate;
        }

        enum class ParameterId
        {
            kFrequency,
            kBypass
        };

        template <typename ProcessContext>
        void process(const ProcessContext& context) noexcept
        {
            auto input = context.getInputBlock();
            auto output = context.getOutputBlock();

            int numSamples = output.getNumSamples();
            // int numChannels = output.getNumChannels();

            auto* leftInputChannel = input.getChannelPointer(0);
            auto* rightInputChannel = input.getChannelPointer(1);

            auto* leftOutputChannel = output.getChannelPointer(0);
            auto* rightOutputChannel = output.getChannelPointer(1);

            if (!m_GlobalBypass)
            {
                for (int i = 0; i < numSamples; ++i)
                {
                    float sineValue = sin(2 * juce::double_Pi * m_frequency * m_time);

                    leftOutputChannel[i] = leftInputChannel[i] + sineValue;
                    rightOutputChannel[i] = rightInputChannel[i] + sineValue;

                    m_time += m_timeDeviation;
                }
            }
            else
            {
                for (int i = 0; i < numSamples; ++i)
                {
                    leftOutputChannel[i] = leftInputChannel[i];
                    rightOutputChannel[i] = rightInputChannel[i];
                }
            }
        }

        SampleType processSample(SampleType input) noexcept
        {
            if (m_GlobalBypass)
            {
                return 0.0;
            }

            float m_LFOValue = sin(2 * juce::double_Pi * m_frequency * m_time);
            m_time += m_timeDeviation;
            return m_LFOValue;
        }

        template <typename ParameterType>
        void changeParameter(ParameterId parameter, ParameterType paramValue)
        {
            switch (parameter)
            {
            case ParameterId::kFrequency: m_frequency = paramValue; break;
            case ParameterId::kBypass: m_GlobalBypass = paramValue; break;
            }
        }

        void reset()
        {

        }

    private:
        float m_frequency;
        float m_time;
        float m_timeDeviation;
        bool m_GlobalBypass { false };
    };
}