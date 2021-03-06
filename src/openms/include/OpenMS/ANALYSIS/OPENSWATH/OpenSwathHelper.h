// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Hannes Roest $
// $Authors: Hannes Roest $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_OPENSWATH_OPENSWATHHELPER_H
#define OPENMS_ANALYSIS_OPENSWATH_OPENSWATHHELPER_H

#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/ANALYSIS/TARGETED/TargetedExperiment.h>
#include <OpenMS/ANALYSIS/OPENSWATH/OPENSWATHALGO/DATAACCESS/TransitionExperiment.h>
#include <OpenMS/ANALYSIS/OPENSWATH/MRMFeatureFinderScoring.h>

namespace OpenMS
{
  /**
    @brief A helper class that is used by several OpenSWATH tools
  */
  class OPENMS_DLLAPI OpenSwathHelper
  {

public:

    /**
      @brief Select transitions between lower and upper and write them into the new TargetedExperiment

      Version for the OpenMS TargetedExperiment
    */
    static void selectSwathTransitions(const OpenMS::TargetedExperiment& targeted_exp,
                                       OpenMS::TargetedExperiment& transition_exp_used, double min_upper_edge_dist,
                                       double lower, double upper);

    /**
      @brief Select transitions between lower and upper and write them into the new TargetedExperiment

      Version for the LightTargetedExperiment
    */
    static void selectSwathTransitions(const OpenSwath::LightTargetedExperiment& targeted_exp,
                                       OpenSwath::LightTargetedExperiment& transition_exp_used, double min_upper_edge_dist,
                                       double lower, double upper);

    /**
      @brief Get the lower / upper offset for this SWATH map and do some sanity checks

     
      Sanity check for the whole map:
       - all scans need to have exactly one precursor
       - all scans need to have the same MS levels (otherwise extracting an XIC
         from them makes no sense)
       - all scans need to have the same precursor isolation window (otherwise
         extracting an XIC from them makes no sense)

      @throw throws IllegalArgument exception if the sanity checks fail.
    */
    static void checkSwathMap(const OpenMS::MSExperiment<Peak1D>& swath_map,
                              double& lower, double& upper);

    /**
      @brief Check the map and select transition in one function
    */
    template <class TargetedExperimentT>
    static bool checkSwathMapAndSelectTransitions(const OpenMS::MSExperiment<Peak1D>& exp,
                                                  const TargetedExperimentT& targeted_exp, TargetedExperimentT& transition_exp_used, double min_upper_edge_dist)
    {
      if (exp.size() == 0 || exp[0].getPrecursors().size() == 0)
      {
        std::cerr << "WARNING: File " << exp.getLoadedFilePath()
                  << " does not have any experiments or any precursors. Is it a SWATH map? "
                  << "I will move to the next map."
                  << std::endl;
        return false;
      }
      double upper, lower;
      OpenSwathHelper::checkSwathMap(exp, lower, upper);
      OpenSwathHelper::selectSwathTransitions(targeted_exp, transition_exp_used, min_upper_edge_dist, lower, upper);
      if (transition_exp_used.getTransitions().size() == 0)
      {
        std::cerr << "WARNING: For File " << exp.getLoadedFilePath()
                  << " no transition were within the precursor window of " << lower << " to " << upper
                  << "I will move to the next map."
                  << std::endl;
        return false;
      }
      return true;

    }

    /**
      @brief Estimate the retention time span of a targeted experiment (returns min/max values as a pair)
    */
    static std::pair<double,double> estimateRTRange(OpenSwath::LightTargetedExperiment & exp);

    /**
      @brief Simple method to extract the best Feature for each transition group (e.g. for RT alignment)

      @param transition_group_map Input data containing the picked and scored map
      @param useQualCutoff Whether to apply a quality cutoff to the data
      @param qualCutoff When applying a quality cutoff, what it should be

      @return Result of the best scoring peaks (stored as map of peptide id and RT)

    */
    static std::map<std::string, double> simpleFindBestFeature(
        OpenMS::MRMFeatureFinderScoring::TransitionGroupMapType & transition_group_map, 
        bool useQualCutoff = false, double qualCutoff = 0.0);

  };
}
#endif
