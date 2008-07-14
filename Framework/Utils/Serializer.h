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
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <string>

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class Serializer
{
public:
    Serializer();
    virtual ~Serializer();

    bool init(int _bufferSize);
    void done();

protected:
    bool mInitd;
    unsigned char * mBuffer;
    int mBufferSize;
    unsigned char * mCurrentPos;
    int mCurrentSize;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class ISerializerIn
{
public:
    virtual std::string     getString() = 0;
    virtual bool            getBool() = 0;
    virtual char            getChar() = 0;
    virtual unsigned char   getUChar() = 0;
    virtual int             getInt() = 0;
    virtual unsigned int    getUInt() = 0;
    virtual float           getFloat() = 0;
    virtual double          getDouble() = 0;
    virtual void            getBuffer(unsigned char * _buffer, int _bufferSize)=0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class ISerializerOut
{
public:
    virtual void addString(std::string _str) = 0;
    virtual void addBool(bool _val) = 0;
    virtual void addChar(char _val) = 0;
    virtual void addUChar(unsigned char _val) = 0;
    virtual void addInt(int _val) = 0;
    virtual void addUInt(unsigned int _val) = 0;
    virtual void addFloat(float _val) = 0;
    virtual void addDouble(double _val) = 0;
    virtual void addBuffer(unsigned char * _buffer, int _bufferSize) = 0;
    virtual void finalize() = 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class SerializerIn : public ISerializerIn, public Serializer
{
public:
    SerializerIn();
    virtual ~SerializerIn();

    // --- Serializer ---
    virtual bool init(int _bufferSize);
    virtual void done();

    // --- ISerializerIn ---
    virtual std::string getString();
    virtual bool getBool();
    virtual char getChar();
    virtual unsigned char getUChar();
    virtual int getInt();
    virtual unsigned int getUInt();
    virtual float getFloat();
    virtual double getDouble();
    virtual void getBuffer(unsigned char * _buffer, int _bufferSize);

protected:
    void get(unsigned char * _ptr, int _byteCount);

    virtual int loadBlock(unsigned char * _ptr, int _count)=0;

private:
    typedef Serializer Inherited;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class SerializerOut : public ISerializerOut, public Serializer
{
public:
    SerializerOut();
    virtual ~SerializerOut();

    virtual bool init(int _bufferSize);
    virtual void done();

    // --- ISerializerOut ---
    virtual void addString(std::string _str);
    virtual void addBool(bool _val);
    virtual void addChar(char _val);
    virtual void addUChar(unsigned char _val);
    virtual void addInt(int _val);
    virtual void addUInt(unsigned int _val);
    virtual void addFloat(float _val);
    virtual void addDouble(double _val);
    virtual void addBuffer(unsigned char * _buffer, int _bufferSize);
    virtual void finalize();

protected:
    typedef Serializer Inherited;

    void add(unsigned char * _ptr, int _byteCount);

    virtual void saveBlock(unsigned char * _ptr, int _count)=0;

private:
    bool mFinalized;
};

#endif // _SERIALIZER_H_

