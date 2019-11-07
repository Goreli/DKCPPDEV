#include <iostream>
#include <typeinfo>
using namespace std;

struct Resource {
	virtual ~Resource() = default;
};

struct Texture : Resource {};

int main() {
	Resource* resource = new Resource;
	Resource* texture = new Texture;

	cout << typeid(*resource).name() << endl; // yields 'Resource'
	cout << typeid(*texture).name() << endl; // yields 'Texture'

	return 0;
}
