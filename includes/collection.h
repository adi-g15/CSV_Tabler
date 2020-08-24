#pragma once

#include<iostream>  //for logging purposes
#include<fstream>
#include<unordered_map>
#include<list>
#include <string>
#include <memory>

class Collection;	//A forward declaration, to use in ObjectId anf Document

class ObjectId{
    size_t counter;
    	//maintains a counter for each Collection, likely a maxAge should be there, but anyways this program is a one time
    ObjectId(){};   //don't instantiate it, in public sense
    auto next() -> size_t;

    public:
        auto count() const -> size_t;

        friend Collection;
};

class Document{
	size_t id;
	std::unordered_map<std::string, std::string> content;
	
	Document(size_t id) : id(id){}

public:
	auto get() const -> decltype((content));	//const map&

	friend Collection;
};

class Collection{
	std::string name;
    size_t docCount;
    ObjectId idCounter;
    std::list<std::string> allKeys;
    std::unordered_map<size_t, std::unique_ptr<Document>> collection; 

    public:
        auto getCount() const -> size_t;
        auto getName() const -> const std::string&;
        auto get(size_t Id) const -> const std::unique_ptr<Document>&;

        auto parseCSV(const std::string&) -> bool;
        auto createDoc(std::string& line, size_t Id) -> bool;	//converts a line of csv to doc

        Collection(std::string n) : name(n), docCount(0){}
};
