#ifndef _ALLJOYN_PERMISSION_MANAGER_H
#define _ALLJOYN_PERMISSION_MANAGER_H
/**
 * @file
 * This file defines the hook to validate whether a message is authorized by the Permission DB.
 */

/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef __cplusplus
#error Only include PermissionManager.h in C++ code.
#endif

#include <alljoyn/PermissionPolicy.h>
#include "PermissionMgmtObj.h"

namespace ajn {

class PermissionManager {

  public:

    /**
     * Constructor
     *
     */
    PermissionManager(BusAttachment& bus) : bus(bus), policy(NULL), guilds(NULL), numOfGuilds(0), permissionMgmtObj(NULL)
    {
    }

    /**
     * virtual destructor
     */
    virtual ~PermissionManager()
    {
        delete policy;
        delete [] guilds;
    }

    void SetPolicy(PermissionPolicy* policy)
    {
        delete this->policy;
        this->policy = policy;
    }

    const PermissionPolicy* GetPolicy()
    {
        return policy;
    }

    /**
     * Set the list of guilds.
     * @param guilds the list of guilds must be new'd by the caller.  It will be deleted by this object.
     */
    void SetGuilds(size_t count, qcc::GUID128* guilds)
    {
        delete [] this->guilds;
        numOfGuilds = count;
        this->guilds = guilds;
    }

    size_t GetNumOfGuilds() {
        return numOfGuilds;
    }

    const qcc::GUID128* GetGuilds()
    {
        return guilds;
    }

    /**
     * Authorize a message.  Make sure there is a proper permission is setup for this type of message.
     * @param send indicating whether is a send or receive
     * @param peerGuid the peer's GUID
     * @param msg the target message
     * @param peerGuildCount the number of guilds the peer belongs to
     * @param peerGuilds the list of guild IDs that the peer belongs to
     * @return
     *  - ER_OK: authorized
     *  - ER_PERM_DENIED: permission denied
     */
    QStatus AuthorizeMessage(bool send, const qcc::GUID128& peerGuid, Message& msg,
                             size_t peerGuildCount, qcc::GUID128* peerGuilds);

    void SetPermissionMgmtObj(PermissionMgmtObj* permissionMgmtObj)
    {
        this->permissionMgmtObj = permissionMgmtObj;
    }

  private:

    bool PeerHasAdminPriv(const qcc::GUID128& peerGuid);
    bool AuthorizePermissionMgmt(bool send, const qcc::GUID128& peerGuid, Message& msg);

    BusAttachment& bus;
    PermissionPolicy* policy;
    qcc::GUID128* guilds;
    size_t numOfGuilds;
    PermissionMgmtObj* permissionMgmtObj;
};

}
#endif