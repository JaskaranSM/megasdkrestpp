//
// Created by zerocool on 12/11/22.
//

#include "megaapi.h"
#include <cstdint>
#include <iostream>

#include "MegaAppRequestListener.h"
#include "log.h"

MegaAppRequestListener::MegaAppRequestListener(mega::MegaApi *api)
{
    this->m_notified = false;
    this->m_api = api;
}

void MegaAppRequestListener::lockAndNotify()
{
    {
        std::unique_lock<std::mutex> lock(this->m_mu);
        m_notified = true;
    }
    m_cv.notify_all();
}

void MegaAppRequestListener::onRequestStart(mega::MegaApi* api, mega::MegaRequest *request)
{
    VLOG_F(2, "onRequestStart");
}

void MegaAppRequestListener::onRequestUpdate(mega::MegaApi* api,mega::MegaRequest *request)
{
    VLOG_F(2, "onRequestUpdate");
}

void MegaAppRequestListener::onRequestFinish(mega::MegaApi* api, mega::MegaRequest *request, mega::MegaError* e)
{
    VLOG_F(2, "onRequestFinish");
    this->m_err = e->copy();
    int requestType = request->getType();
    if(e->getErrorCode() != mega::MegaError::API_OK)
    {
        VLOG_F(2, "onRequestFinish: E: %s", e->getErrorString());
        this->lockAndNotify();
    } else {
        switch(requestType)
        {
            case mega::MegaRequest::TYPE_LOGIN:
                VLOG_F(2, "onRequestFinish: request type is login, starting api->fetchNodes operation");
                api->fetchNodes(this);
                break;
            case mega::MegaRequest::TYPE_FETCH_NODES:
                VLOG_F(2, "onRequestFinish: request type is fetchNodes, setting up rootNode");
                this->m_publicNode = api->getRootNode()->copy();
                break;
            case mega::MegaRequest::TYPE_GET_PUBLIC_NODE:
                VLOG_F(2, "onRequestFinish: request type is getPublicNode, setting publicNode");
                this->m_publicNode = request->getPublicMegaNode()->copy();
                break;
        }
    }

    if(requestType != mega::MegaRequest::TYPE_LOGIN && requestType != mega::MegaRequest::TYPE_DELETE)
    {
        VLOG_F(2, "onRequestFinish: calling lockAndNotify");
        this->lockAndNotify();
    }
}

void MegaAppRequestListener::onRequestTemporaryError(mega::MegaApi* api, mega::MegaRequest *request, mega::MegaError* error)
{
    VLOG_F(2, "onRequestTemporaryError: %s", error->toString());
}

mega::MegaNode* MegaAppRequestListener::GetPublicNode()
{
    return this->m_publicNode;
}

void MegaAppRequestListener::SetPublicNode(mega::MegaNode *node)
{
    this->m_publicNode = node;
}

mega::MegaError* MegaAppRequestListener::GetError()
{
    return this->m_err;
}

void MegaAppRequestListener::Reset()
{
    this->m_notified = false;
}

void MegaAppRequestListener::Wait()
{
    VLOG_F(2, "Wait: waiting for condition_variable to be notified");
    std::unique_lock<std::mutex> lock(this->m_mu);
    this->m_cv.wait(lock, [this]{return this->m_notified;});
}