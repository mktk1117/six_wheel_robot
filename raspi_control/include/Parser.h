#pragma once
#include <sio_client.h>
#include <Object.h>


int64_t intVal;
double doubleVal;
double vec3[3];
std::string stringVal;
void parseSampleData(sio::message::ptr data);
void parseVec3(sio::message::ptr data);

void parseSampleData(sio::message::ptr data){
	sio::message::ptr ptr;
	/*bool b = ptr->get_bool();
	int64_t i = ptr->get_int();
	double d = ptr->get_double();
	std::string s = ptr->get_string();
	std::vector<sio::message::ptr> vector = ptr->get_vector();
	std::map<std::string, sio::message::ptr> map = ptr->get_map();*/

	std::map<std::string, sio::message::ptr> map = data->get_map();//最外側のObjectからmapを抽出
	intVal = map["Int"]->get_int();
	doubleVal = map["Double"]->get_double();
	std::vector<sio::message::ptr> vector = map["Array"]->get_vector();
	for (int i = 0; i < 3; ++i){
		vec3[i] = vector[i]->get_double();
	}
	stringVal = vector[3]->get_string();
}

// sample function to get vector data
void parseVec3(sio::message::ptr data){
	std::vector<sio::message::ptr> vector = data->get_vector();
	for (int i = 0; i < 3; ++i){
		vec3[i] = vector[i]->get_double();
	}
}
