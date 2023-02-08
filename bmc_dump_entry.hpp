#pragma once

#include "bmcstored_dump_entry.hpp"
#include "xyz/openbmc_project/Dump/Entry/BMC/server.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>

#include <filesystem>

namespace phosphor
{
namespace dump
{
namespace bmc
{
template <typename T>
using ServerObject = typename sdbusplus::server::object_t<T>;

using EntryIfaces = sdbusplus::server::object_t<
    sdbusplus::xyz::openbmc_project::Dump::Entry::server::BMC,
    sdbusplus::xyz::openbmc_project::Common::server::FilePath>;

using originatorTypes = sdbusplus::xyz::openbmc_project::Common::server::
    OriginatedBy::OriginatorTypes;

class Manager;

/** @class Entry
 *  @brief OpenBMC Dump Entry implementation.
 *  @details A concrete implementation for the
 *  xyz.openbmc_project.Dump.Entry DBus API
 */
class Entry :
    virtual public EntryIfaces,
    virtual public phosphor::dump::bmc_stored::Entry
{
  public:
    Entry() = delete;
    Entry(const Entry&) = delete;
    Entry& operator=(const Entry&) = delete;
    Entry(Entry&&) = delete;
    Entry& operator=(Entry&&) = delete;
    ~Entry() = default;

    /** @brief Constructor for the Dump Entry Object
     *  @param[in] bus - Bus to attach to.
     *  @param[in] objPath - Object path to attach to
     *  @param[in] dumpId - Dump id.
     *  @param[in] timeStamp - Dump creation timestamp
     *             since the epoch.
     *  @param[in] fileSize - Dump file size in bytes.
     *  @param[in] file - Name of dump file.
     *  @param[in] status - status of the dump.
     *  @param[in] originatorId - Id of the originator of the dump
     *  @param[in] originatorType - Originator type
     *  @param[in] parent - The dump entry's parent.
     */
    Entry(sdbusplus::bus_t& bus, const std::string& objPath, uint32_t dumpId,
          uint64_t timeStamp, uint64_t fileSize,
          const std::filesystem::path& file,
          phosphor::dump::OperationStatus status, std::string originatorId,
          originatorTypes originatorType, phosphor::dump::Manager& parent) :
        EntryIfaces(bus, objPath.c_str(), EntryIfaces::action::defer_emit),
        phosphor::dump::bmc_stored::Entry(bus, objPath.c_str(), dumpId,
                                          timeStamp, fileSize, file, status,
                                          originatorId, originatorType, parent)
    {
        // Emit deferred signal.
        this->phosphor::dump::bmc::EntryIfaces::emit_object_added();
    }
};

} // namespace bmc
} // namespace dump
} // namespace phosphor
