/* 
 * Copyright 2007-2008 InCrew Software, All Rights Reserved
 *
 * This file may be used or modified without the need for a license.
 *
 * Redistribution of this file in either its original form, or in an
 * updated form may be done under the terms of the GNU LIBRARY GENERAL
 * PUBLIC LICENSE.  If this license is unacceptable to you then you
 * may not redistribute this work.
 * 
 * See the file COPYING.GPL for details.
 */
#include "PchNWStream.h"

#include "NWStreamBlockQueue.h"
#include "NWCriticalSection.h"
#include "NWEvent.h"
#include "SystemUtils.h"
#include "INWStreamBlock.h"
#include "NWStream.h"
#include "NWStreamGroup.h"

const int MAX_SIZE_WRITE_QUEUE = 1;

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
NWStreamBlockQueue::NWStreamBlockQueue() :
    mInit(false),
    mWriteBuffer(0),
    mCSSwap(0),
    mEventNewData(0),
    mEventMaxSize(0),
    mDisableRead(false),
    mDisableWrite(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool NWStreamBlockQueue::init(NWStreamWriter* _stream)
{
    bool bOK = true;

    if (!isOk())
    {
        mStream = _stream;
        mWriteBuffer = 0;
        mNeedEvent = false;
        mDisableRead = false;
        mDisableWrite = false;
        mCSSwap = NWCriticalSection::create();
        mEventNewData = NWEvent::create();
        mEventMaxSize = NWEvent::create();

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockQueue::done()
{
    if (isOk())
    {
        std::list<INWStreamBlock*>& writeBlocks = getWriteBuffer();
        while ( writeBlocks.size() > 0 )
        {
            INWStreamBlock* block = writeBlocks.front();
            DISPOSE(block);
            writeBlocks.pop_front();
        }

        NWCriticalSection::destroy(mCSSwap);
        NWEvent::destroy(mEventNewData);
        NWEvent::destroy(mEventMaxSize);
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockQueue::writeBlock(INWStreamBlock* _block)
{
    mCSSwap->enter();

    LOG("Write block %s (%d)",mStream->getStreamGroupWrite()->getName(),rand());

    //unsigned int threadId = SystemUtils::getCurrentThreadId();
    //LOG("Write Thread id (%d) (0x%08x)", threadId, this);

    std::list<INWStreamBlock*>& writeBlocks = getWriteBuffer();
    
    if ( !mDisableWrite )
    {
        if ( writeBlocks.size() > MAX_SIZE_WRITE_QUEUE )
        {
            mCSSwap->leave();
            mEventMaxSize->waitForSignal();
            mCSSwap->enter();
        }
    }

    if ( !mDisableWrite )
    {
        writeBlocks.push_back(_block);

        if ( mNeedEvent )
        {
            mNeedEvent = false;
            ASSERT(writeBlocks.size() > 0);
            mEventNewData->signal();
        }
    }
    else
    {
        DISPOSE(_block);
    }

    mCSSwap->leave();

    /*mCSSwap->enter();
    
    std::list<INWStreamBlock*>& writeBlocks = getWriteBuffer();
    if ( (int)writeBlocks.size() > MAX_SIZE_WRITE_QUEUE )
    {
        mCSSwap->leave();
        mEventMaxSize->waitForSignal();
        mCSSwap->enter();
        writeBlocks = getWriteBuffer();
    }
    writeBlocks.push_back(_block);

    if ( mNeedEvent )
    {
        mNeedEvent = false;
        mEventNewData->signal();
    }
    
    mCSSwap->leave();*/
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
INWStreamBlock* NWStreamBlockQueue::readBlock()
{   
    mCSSwap->enter();

    //LOG("Read block %s (%d)",mStream->getStreamGroupRead()->getName(),rand());

    INWStreamBlock* block = 0;
    if ( !mDisableRead )
    {
        //unsigned int threadId = SystemUtils::getCurrentThreadId();
        //LOG("Read Thread id (%d) (0x%08x)", threadId, this);

        std::list<INWStreamBlock*>& writeBlocks = getWriteBuffer();
        
        if ( writeBlocks.size() > MAX_SIZE_WRITE_QUEUE )
            mEventMaxSize->signal();

        if ( writeBlocks.size() == 0 )
        {
            mNeedEvent = true;
            mCSSwap->leave();
            mEventNewData->waitForSignal();
            mCSSwap->enter();        
        }

        if ( !mDisableRead )
        {
            ASSERT(writeBlocks.size() > 0);

            block = writeBlocks.front();
            writeBlocks.pop_front();
        }
    }

    mCSSwap->leave();


    /*std::list<INWStreamBlock*>& readBlocks = getReadBuffer();

    // We must swap the buffers if we've not blocks queued in the read buffer
    if ( readBlocks.size() == 0 )
    {
        swap();
        readBlocks = getReadBuffer();
    }

    // Get first block from the read buffer
    ASSERT(readBlocks.size() > 0);
    INWStreamBlock* block = readBlocks.front();
    readBlocks.pop_front();*/
    
    return block;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*void NWStreamBlockQueue::swap()
{
    mCSSwap->enter();

    // swap buffers
    mWriteBuffer = (mWriteBuffer+1)&1;

    // If we've not new data in the read buffer, we set the "send event
    // on new message" and wait until a new message be queued
    std::list<INWStreamBlock*>& readBlocks = getReadBuffer();
    if ( readBlocks.size() == 0 )
    {
        mNeedEvent = true;
        mEventMaxSize->signal();
        mCSSwap->leave();
        mEventNewData->waitForSignal();
        mCSSwap->enter();
        // swap buffers again
        mWriteBuffer = (mWriteBuffer+1)&1;
        // re-get the read buffer
        readBlocks = getReadBuffer();
    }

    ASSERT(readBlocks.size() > 0);

    mCSSwap->leave();
}*/

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockQueue::disableRead(bool _disable)
{
    mCSSwap->enter();
    if ( _disable != mDisableRead )
    {
        mDisableRead = _disable;
        if ( mDisableRead )
        {
            mEventNewData->signal();
        }
        else
        {
            mEventNewData->reset();
        }
    }
    mCSSwap->leave();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NWStreamBlockQueue::disableWrite(bool _disable)
{
    mCSSwap->enter();
    if ( _disable != mDisableWrite )
    {
        mDisableWrite = _disable;
        if ( mDisableWrite )
        {
            mEventMaxSize->signal();
        }
        else
        {
            mEventMaxSize->reset();
        }
    }
    mCSSwap->leave();
}
