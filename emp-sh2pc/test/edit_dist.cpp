#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;

void test_edit_dist(int bitsize, string inputs_a[], string inputs_b[]) {
  //Integer product(bitsize, 1);
	Integer sum(3, 0, PUBLIC);	

	Integer a[5];
	Integer b[5];
  for (int i=0; i<5; i++) {
    a[i] = Integer(bitsize, inputs_a[i], ALICE);
    b[i] = Integer(bitsize, inputs_b[i], BOB);
	}

    Integer one (3, 1, PUBLIC);
    Integer zero (3, 0, PUBLIC);
    for( int i=0; i < 5; i++ ) {
	   
	   Bit s = a[i].equal(b[i]);
	   cout << s.reveal();
           cout << a[i].reveal<int>();
	   cout << b[i].reveal<int>();
	   sum = sum + one.select(s, zero);
    // reconstruct "secret shared" inputs
    //a[i] = a[i] + b[i];
    // multiply value into product
    //product = product * a[i];
  }

  cout << "Edit Distance (binary notation): ";
  for(int i=3-1; i>=0; i--) {
      cout << sum[i].reveal();
  }
  //cout << sum.reveal();
  cout << endl;
}

int main(int argc, char** argv) {
    int bitsize;

    // generate circuit for use in malicious library
    if (argc == 2 && strcmp(argv[1], "-m") == 0 ) {
        setup_plain_prot(true, "edit_dist.circuit.txt");
        bitsize = 16;
        string inputs[3] = {"0","0","0"};
        test_edit_dist(bitsize, inputs, inputs);
        finalize_plain_prot();
	return 0;
    }
    
    // run computation with semi-honest model
    int port, party;
    parse_party_and_port(argv, &party, &port);
    NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

    setup_semi_honest(io, party);

    if (argc != 4) {
      cout << "Usage: ./mult3 <party> <port> <bitsize>" << endl
           << "where <value> are secret shares of the three inputs"
           << endl;
      delete io;
      return 0;
    }

    bitsize = atoi(argv[3]);
    char fname_a[40];
    char fname_b[40];

    sprintf(fname_a, "../data/edit_dist/%d.1.dat", 32);
    sprintf(fname_b, "../data/edit_dist/%d.2.dat", 32);

    ifstream infile_a(fname_a);
    ifstream infile_b(fname_b);

    string inputs_a[5];
    string inputs_b[5];

    if( infile_a.is_open() && infile_b.is_open()) {
        for( int i=0; i<5; i++) {
            getline( infile_a, inputs_a[i]);
            getline( infile_b, inputs_b[i]);
        }
        infile_a.close();
        infile_b.close();
    }

    test_edit_dist(bitsize, inputs_a, inputs_b);
    cout << inputs_a[1];
    cout << inputs_b[1];
    cout << endl;
    delete io;
}

