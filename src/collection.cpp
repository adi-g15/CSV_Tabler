#include "collection.h"
#include <vector>
#include <thread>
#include <mutex>

std::mutex gMutex;

auto ObjectId::next() -> size_t{
	gMutex.lock();
	++ObjectId::counter;	//incrementing counter
	gMutex.unlock();

	return ObjectId::count();
}

auto ObjectId::count() const -> size_t{
	std::lock_guard<std::mutex> lockGaurd(gMutex);	//or could have used .lock() or unlock()

	return ObjectId::counter;
}

auto Document::get() const -> decltype((this->content)){
	return this->content;
}

auto Collection::getName() const -> const std::string&{
	return this->name;
}

auto Collection::getCount() const -> size_t{
	return this->docCount;
}

auto Collection::get(size_t Id) const -> const std::unique_ptr<Document>&{
	if(Id > this->idCounter.count()){
		throw std::invalid_argument("Invalid ID: More than current count of documents");
	}
	return this->collection.at(Id);
}

auto Collection::parseCSV(const std::string& path) -> bool{
	std::ifstream fin;
	fin.open(path);

	std::vector<std::thread> threads;
	std::string line;

	getline(fin, line, '\n');
	

	// while(!fin.eof()){
	// 	getline(fin, line, '\n');

	// 	threads.push_back( std::thread(&Collection::createDoc, line, this->idCounter.next()) );
	// }

	// for(auto &&t : threads){
	// 	if( t.joinable() )
	// 		t.join();
	// }
	// threads.clear();
	fin.close();
	return true;
}

auto Collection::createDoc(std::string &line, size_t Id) -> bool{
	auto doc = std::unique_ptr<Document>(new Document(Id));


}
