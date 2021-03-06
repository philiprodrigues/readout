/**
 * @file ElinkReader.hpp FELIX FULLMODE Link reader module
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
*/
#ifndef UDAQ_READOUT_SRC_ELINKREADER_HPP_
#define UDAQ_READOUT_SRC_ELINKREADER_HPP_

#include "appfwk/DAQModule.hpp"

// 3rd party felix
#include "packetformat/block_parser.hpp"

// module
#include "ReadoutTypes.hpp"
#include "ReusableThread.hpp"
#include "DefaultParserImpl.hpp"

#include <string>
#include <chrono>
#include <vector>

namespace dunedaq {
namespace readout {

class ElinkReader : public dunedaq::appfwk::DAQModule
{
public:
  /**
  * @brief ElinkReader Constructor
  * @param name Instance name for this ElinkReader instance
  */
  explicit ElinkReader(const std::string& name);

  ElinkReader(const ElinkReader&) =
    delete; ///< ElinkReader is not copy-constructible
  ElinkReader& operator=(const ElinkReader&) =
    delete; ///< ElinkReader is not copy-assignable
  ElinkReader(ElinkReader&&) =
    delete; ///< ElinkReader is not move-constructible
  ElinkReader& operator=(ElinkReader&&) =
    delete; ///< ElinkReader is not move-assignable

  void init(const data_t&) override;

private:
  // Commands
  void do_start(const data_t& /*args*/);
  void do_stop(const data_t& /*args*/);

  // Configuration
  bool configured_;
  types::UniqueBlockPtrSource block_ptr_source_;
  types::UniqueWIBFrameSink parser_sink_;
  std::chrono::milliseconds source_timeout_;

  // Internals
  std::string info_str_;
  unsigned link_id_;
  unsigned link_tag_;
  std::string parser_type_;
  std::string id_str_;
 
  // Processor
  inline static const std::string dma_processor_name_ = "flx-parser";
  ReusableThread link_processor_;
  std::atomic<bool> run_lock_{false};
  std::atomic<bool> active_{false};

  // Operations
  DefaultParserImpl parser_impl_;
  felix::packetformat::BlockParser<DefaultParserImpl> parser_;
  void processLink();

};

} // namespace dunedaq::readout
}

#endif // UDAQ_READOUT_SRC_ELINKREADER_HPP_
