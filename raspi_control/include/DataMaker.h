#pragma once
#include <sio_client.h>
#include <Object.h>

// Sample functions to make data
sio::Null makeSampleNull();//Null
sio::Boolean makeSampleBoolean();//bool
sio::Integer makeSampleInteger();//int
sio::Float makeSampleFloat();//float
sio::String makeSampleString();//string
sio::Array makeSampleArray();//array
sio::Object makeSampleObject();//dictionary

// Make cmd_vel
sio::Object makeCmdVel();

sio::Null makeSampleNull(){
	return sio::Null();
}

sio::Boolean makeSampleBoolean(){
	return sio::Boolean(true);
}

sio::Integer makeSampleInteger(){
	return sio::Integer(555);
}

sio::Float makeSampleFloat(){
	return sio::Float(1e8);
}

sio::String makeSampleString(){
	return sio::String("string data");
}

sio::Array makeSampleArray(){
	sio::Array ret;
	ret.add((int64_t)1)
		.add("string")
		.add(false)
		.add(sio::Array().add(3.0).add(4.0).add(5.0));
	return ret;
}
sio::Object makeSampleObject(){
	sio::Object ret;
	ret.addInt("message", 1323)
            .add("string", "stringtest")
		.add("Int1", (int64_t)1)
		.addInt("Int2", 2)
		.add("Double", 1.0 / 3.0)
		.add("String", "C++")
		.add("Array", sio::Array()
		.addInt(1)
		.add((int64_t)2)
		.add("str"))
		.add("Object", sio::Object()
		.add("Key", "Value"));
	return ret;
}

// make cmd_vel
sio::Object makeCmdVel(double linearx, double lineary, double linearz, double angularx, double angulary, double angularz){
    sio::Object linear;
    linear.add("x", linearx)
        .add("y", lineary)
        .add("z", linearz);
    sio::Object angular;
    angular.add("x", angularx)
        .add("y", angulary)
        .add("z", angularz);
    sio::Object ret;
    ret.add("linear", linear)
        .add("angular", angular);
    return ret;
}






