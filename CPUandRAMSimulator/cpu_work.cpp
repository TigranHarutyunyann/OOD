#include <iostream>
#include <map>
#include <memory>
using namespace std;

// ---------------- RAM Interface ----------------
class RAMInterface {
public:
    virtual int get(int addr) = 0;
    virtual void set(int addr, int val) = 0;
    virtual ~RAMInterface() {}
};

// ---------------- Concrete RAM ----------------
class RAM : public RAMInterface {
private:
    map<int,int> mem;
public:
    void loadData(int addr, int val) { mem[addr] = val; }
    int get(int addr) override { return mem[addr]; }
    void set(int addr, int val) override { mem[addr] = val; }
};

class CPU;

// ---------------- Abstract Command ----------------
class ICommand {
public:
    virtual void execute(CPU& cpu, RAMInterface& ram, map<int, shared_ptr<ICommand>>& RAMmap) = 0;
    virtual ~ICommand() {}
};

// ---------------- CPU Class ----------------
class CPU {
private:
    int PC = 0;
    int ACC = 0;
public:
    void setPC(int val) { PC = val; }
    int getPC() { return PC; }
    void incrementPC() { PC++; }

    void setACC(int val) { ACC = val; }
    int getACC() { return ACC; }

    void run(map<int, shared_ptr<ICommand>>& RAMmap, RAMInterface& RAM) {
        PC = 0;
        ACC = 0;

        while (true) {
            if (RAMmap.find(PC) == RAMmap.end()) {
                cout << "No command at PC=" << PC << endl;
                break;
            }
            auto cmd = RAMmap[PC];
            PC++;
            cmd->execute(*this, RAM, RAMmap);
        }
    }
};

// ---------------- Concrete Commands ----------------
class LDACommand : public ICommand {
    int addr;
public:
    LDACommand(int a) : addr(a) {}
    void execute(CPU& cpu, RAMInterface& ram, map<int, shared_ptr<ICommand>>&) override {
        cpu.setACC(ram.get(addr));
		cout << "\nPC: " << cpu.getPC() << " |ACC: " << cpu.getACC() << endl;
    }
};

class SUBCommand : public ICommand {
    int addr;
public:
    SUBCommand(int a) : addr(a) {}
    void execute(CPU& cpu, RAMInterface& ram, map<int, shared_ptr<ICommand>>&) override {
        cpu.setACC(cpu.getACC() - ram.get(addr));
		cout << "\nPC: " << cpu.getPC() << " |ACC: " << cpu.getACC() << endl;
    }
};

class STACommand : public ICommand {
    int addr;
public:
    STACommand(int a) : addr(a) {}
    void execute(CPU& cpu, RAMInterface& ram, map<int, shared_ptr<ICommand>>&) override {
        ram.set(addr, cpu.getACC());
		cout << "\nPC: " << cpu.getPC() << " |ACC: " << cpu.getACC() << endl;
    }
};

class BRZCommand : public ICommand {
    int addr;
public:
    BRZCommand(int a) : addr(a) {}
    void execute(CPU& cpu, RAMInterface&, map<int, shared_ptr<ICommand>>&) override {
        if (cpu.getACC() == 0) cpu.setPC(addr);
		cout << "\nPC: " << cpu.getPC() << " |ACC: " << cpu.getACC() << endl;
    }
};

class BRACommand : public ICommand {
    int addr;
public:
    BRACommand(int a) : addr(a) {}
    void execute(CPU& cpu, RAMInterface&, map<int, shared_ptr<ICommand>>&) override {
        cpu.setPC(addr);
		cout << "\nPC: " << cpu.getPC() << " |ACC: " << cpu.getACC() << endl;
    }
};

class HLTCommand : public ICommand {
public:
    void execute(CPU& cpu, RAMInterface&, map<int, shared_ptr<ICommand>>&) override {
		cout << "\nPC: " << cpu.getPC() << " |ACC: " << cpu.getACC() << endl;
        cout << "Program halted." << endl;
        exit(0);
    }
};

// DAT command (data only)
class DATCommand : public ICommand {
public:
    void execute(CPU&, RAMInterface&, map<int, shared_ptr<ICommand>>&) override { }
};

// ---------------- Testcase ----------------
void runTestcase(map<int, shared_ptr<ICommand>> RAMmap, RAM& RAMmem, int expectedACC, const string& name) {
    CPU cpu;
    cpu.run(RAMmap, RAMmem);
    int result = cpu.getACC();
    cout << name << ": Expected ACC=" << expectedACC
         << ", got ACC=" << result
         << " -> " << (result == expectedACC ? "PASS" : "FAIL") << endl;
}

// ---------------- Main ----------------
int main() {
    RAM RAMmem;

    // Initialize RAM data
    RAMmem.loadData(10, 10);
    RAMmem.loadData(80, 5000);

    // Initialize RAM commands
    map<int, shared_ptr<ICommand>> RAMmap;
    RAMmap[0] = make_shared<LDACommand>(80);
    RAMmap[1] = make_shared<SUBCommand>(10);
    RAMmap[2] = make_shared<STACommand>(80);
    RAMmap[3] = make_shared<BRZCommand>(5);
    RAMmap[4] = make_shared<BRACommand>(0);
    RAMmap[5] = make_shared<HLTCommand>();
    RAMmap[10] = make_shared<DATCommand>();
    RAMmap[80] = make_shared<DATCommand>();

    // Run the CPU
    CPU cpu;
    cpu.run(RAMmap, RAMmem);

    return 0;
}

