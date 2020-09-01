#include "TestSlot.h"
#include "base/sigslot.h"
#include <stdio.h>
#include <string>
#include <vector>

class Light : public sigslot::has_slots<> {
public:
    void turnLight(int arg, std::string say) {
        printf("turnLight arg %d say %s\n", arg, say.c_str());
    }

};

class Switch {

public:
    sigslot::signal2<int, std::string> click;

};

void TestSlot::setUp() {
    printf("TestSlot::setUp \n");
}

void TestSlot::run() {

    Light mLight;
    Switch mSwitch;

    mSwitch.click.connect(&mLight, &Light::turnLight);

    std::string hey = "hey from slot";
    mSwitch.click.emit(1, hey);

    mSwitch.click.disconnect_all();
}

void TestSlot::tearDown() {
    printf("TestSlot::tearDown \n\n");
}
