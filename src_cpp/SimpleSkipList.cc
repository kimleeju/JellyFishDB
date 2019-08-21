#include "SimpleSkipList.h"

int SimpleSkipList::put(string key, string value)
{
	//cout << __func__ << " ( " << key << " " << value << " )..." << endl;
	printf("put( %s, %s )...\n", key.c_str(), value.c_str());
	kv_map.insert(make_pair(key, value));
	return 1;
}

string SimpleSkipList::get(string key)
{
	//cout << __func__ << " ( " << key << " )..." << endl;
	printf("get( %s )...\n", key.c_str());
	multimap<string, string>::iterator iter;
	iter = kv_map.find(key);
	if (iter == kv_map.end()) { // not found
		return "not found";
	}
	else { // found
		string val = iter->second;
		return val;
	}
}

int SimpleSkipList::remove(string key)
{
	cout << __func__ << endl;
	return 1;
}

// iterator 사용해서 find로 탐색
// 탐색한거부터 n개만큼 읽어온 map 리턴 하면 ok
multimap<string, string> SimpleSkipList::range_query(string starting_key, int n) {
	//cout << __func__ << endl;
	printf("scan( %s  count %d )...\n", starting_key.c_str(), n);
	multimap<string, string> m;
	multimap<string, string>::iterator iter;
	int count = n;

	iter = kv_map.find(starting_key);

	// starting_key를 찾으면
	if (iter != kv_map.end()) {
		for (; (iter != kv_map.end()) && (count > 0); iter++, count--) {
			m.insert(make_pair(iter->first, iter->second)); // n 개수 만큼 iterator로 접근해서 m에 저장
		}
	}
	else {  // 못찾으면
		cout << "key " << starting_key << "는 존재하지 않습니다." << endl;
	}
	return m;
}

void SimpleSkipList::print() {
	multimap<string, string>::iterator p;

	cout << "[print kv_map]" << endl;
	for (p = kv_map.begin(); p != kv_map.end(); p++) {
		cout << "key : " << p->first << "  value : " << p->second << endl;
	}

}

