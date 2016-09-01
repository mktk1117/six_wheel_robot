/*
 * Object.h
 *  Created on: 2016/04/23
 *      Author: T-Kawai0922
 */

#pragma once
#include <string>
#include <stdint.h>
#include <map>
#include <sio_client.h>
namespace sio{
	class Null;
	class Boolean;
	class Integer;
	class Float;
	class String;
	class Binary;
	class Array;
	class Object;
	//Null
	class Null{
		sio::message::ptr data;
	public:
		Null();
		virtual ~Null();
		sio::message::ptr pack(void) const;
	};
	//Boolean
	class Boolean{
		sio::message::ptr data;
	public:
		Boolean(const bool& value);
		virtual ~Boolean();
		sio::message::ptr pack(void) const;
	};
	//Integer
	class Integer{
		sio::message::ptr data;
	public:
		Integer(const int64_t& value);
		virtual ~Integer();
		sio::message::ptr pack(void) const;
	};
	//Float
	class Float{
		sio::message::ptr data;
	public:
		Float(const double& value);
		virtual ~Float();
		sio::message::ptr pack(void) const;
	};
	//String
	class String{
		sio::message::ptr data;
	public:
		String(const std::string& value);
		virtual ~String();
		sio::message::ptr pack(void) const;
	};
	//Binary
	/*class Binary{
		sio::message::ptr data;
	public:
		Binary(const bool& value);
		virtual ~Binary();
		sio::message::ptr pack(void) const;
	};*/
	
	class Array{
		sio::message::ptr data;
	public:
		Array();
		virtual ~Array();
		Array& add(const Null& value);
		Array& add(const bool& value);
		Array& addInt(const int64_t& value);
		Array& add(const int64_t& value);
		Array& add(const double& value);
		Array& add(const std::string& value);
		Array& add(const Object& value);
		Array& add(const Array& value);
		sio::message::ptr pack(void) const;
	};
	class Object {
		sio::message::ptr data;
	public:
		Object();
		virtual ~Object();
		Object& add(const std::string& key,const Null& value);
		Object& add(const std::string& key,const bool& value);
		Object& addInt(const std::string& key,const int64_t& value);
		Object& add(const std::string& key,const int64_t& value);
		Object& add(const std::string& key,const double& value);
		Object& add(const std::string& key,const std::string& value);
		Object& add(const std::string& key,const Object& value);
		Object& add(const std::string& key,const Array& value);
		sio::message::ptr pack(void) const;
		static const std::map<std::string,sio::message::ptr>& parse(sio::event& event);
		std::string toString(void) const;
	};
	std::ostream& operator<<(std::ostream& os, const sio::message::ptr& data);
};

