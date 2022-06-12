struct ANSWER_CNAME_RDATA_TYPE {
	string name;
};

struct ANSWER_A_RDATA_TYPE {
	uint8_t ip4_p1;
	uint8_t ip4_p2;
	uint8_t ip4_p3;
	uint8_t ip4_p4;
};

struct ANSWER_AAAA_RDATA_TYPE {
	uint8_t ip6_p1;
	uint8_t ip6_p2;
	uint8_t ip6_p3;
	uint8_t ip6_p4;
	uint8_t ip6_p5;
	uint8_t ip6_p6;
	uint8_t ip6_p7;
	uint8_t ip6_p8;

};

union ANSWER_RDATA_TYPE {
	struct ANSWER_CNAME_RDATA_TYPE cname_rdata;
	struct ANSWER_A_RDATA_TYPE a_rdata;
	struct ANSWER_AAAA_RDATA_TYPE aaaa_rdata;
};

class Section {
public:
	virtual int parse() = 0;
	virtual int display() = 0;
};

class Header: public Section {
private:
	const int HEADER_SIZE_BYTES = 12;
	uint16_t id;
	uint16_t qr;
	uint16_t opcode;
	uint16_t aa;
	uint16_t tc;
	uint16_t rd;
	uint16_t ra;
	// uint16_t res1;
	// uint16_t res2;
	// uint16_t res3;
	uint16_t rcode;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;

public:
	Header() {
		id = 0; qr = 0; opcode = 0; aa = 0; tc = 0; rd = 0; ra = 0;
		rcode = 0; qdcount = 0; ancount = 0; nscount = 0; arcount = 0;
	}
	// number of question sections
	// number of answer sections
	// number of authority sections
	// number of additional sections

	// returns the end of the header section in encoded_message
	int parse(string &encoded_message, int &start_index) {
		// convert the input string to 16bit words
		vector<uint16_t> words;

		Utils ufuncs;
		if (encoded_message.size() > HEADER_SIZE_BYTES)
			return -1;

		 // parse the header section
		for (int idx = start_index; idx < start_index +HEADER_SIZE_BYTES * 2;idx += 4) {
			uint16_t word = 0;
			for (int word_octate = 0; word_octate < 4; ++word_octate)
				word = word * 16 + ufuncs.hex_to_dig(
									encoded_message[idx + word_octate]);
			words.push_back(word);
		}

		// then populate the variables
		id = words[0];
		qr = words[1] & (1 << 15);
		opcode = words[1] & (0xf << 11);
		aa = words[1] & (1 << 10);
		tc = words[1] & (1 << 9);
		rd = words[1] & (1 << 8);
		ra = words[1] & (1 << 7);
		rcode = words[1] & (0xf);
		qdcount = words[2];
		ancount = words[3];
		nscount = words[4];
		arcount = words[5];

		return start_index + HEADER_SIZE_BYTES * 2;
	}
	int display() {
		cout<<";;->>HEADER<<- ";
		cout<<";;->>opcode: <<- ";
		(opcode == 1U) ? cout<<"answer":cout<<"query";

		cout<<"status: "
		switch (rcode) {
			case 0:
				cout<<"NOERROR";
				break;
			case 1:
				cout<<"FORMAT ERROR";
				break;
			case 2:
				cout<<"SERVER ERROR";
				break;
			case 3:
				cout<<"NAME ERROR";
				break;
			case 4:
				cout<<"NOT IMPLEMENTED";
				break;
			case 5:
				cout<<"REFUSED";
				break;
			default:
				cout<<"deafult";
				break;
		}

		cout<<"id: "<<id;

		cout<<"\n;; flags:";
		if (aa == 1U)
			cout<<" aa";
		if (tc == 1U)
			cout<<" tc";
		if (rd == 1U)
			cout<<" rd";
		if (tc == 1U)
			cout<<" ra";

		cout<<";";

		cout<<"QUERY: "<<qdcount<<",";
		cout<<"ANSWER: "<<ancount<<",";
		cout<<"AUTHORITY: "<<nscount<<",";
		cout<<"ADDITONAL: "<<arcount;
		cout<<endl;

		return HEADER_SIZE_BYTES * 2U;
	}
	uint16_t num_qs() {
		return qdcount;
	}
	uint16_t num_an() {
		return ancount;
	}
	uint16_t num_ns() {
		return nscount;
	}
	uint16_t num_ar() {
		return arcount;
	}
};

class QuestionSection: public Section{
private:
	string qname;
	uint16_t qclass;
	uint16_t qtype;
public:
	int parse(string &encoded_message, int &start_index) {
		uint8_t qtype = 0;
		Utils ufuncs;
		// read the qname QTYPE
		uint16_t idx =  start_index;

		qtype = encoded_message[idx];
		qtype = encoded_message[idx + 1];

		idx += 2;
		if (qtype == 0U) {
			idx = ufuncs.labels_parser(encoded_message, idx, qname);
		} else {

		}

		// QTYPE
		for (int q_index = idx; q_index < (idx + 4); ++q_index)
			qtype = qtype * 10 +
					ufuncs.encoded_message(encoded_message[q_index]);

		idx += 4;
		// QCLASS
		for (int q_index = idx; q_index < (idx + 4); ++q_index)
			qclass = qclass * 10 +
					ufuncs.encoded_message(encoded_message[q_index]);

		// same logic as above
	}
	int display() {
		// print question url
		cout<<";;->>QUESTION SECTION<<- ";
		cout<<qname<<"      ";
		// qtype
		switch(qtype) {
			case 1U:
				cout<<"     A";
			case 5U:
				cout<<"     CNAME";

			case 0x1CU:
				cout<<"     AAAA";
		}

		// qclass print
		switch(qclass) {
			case 1U:
				cout<<"     IN"
		}
		cout<<endl;
	}
};

class AnswerSection: public Section {
private:
	string aname;
	uint16_t type;
	uint32_t ttl;
	uint8_t an_class;
	uint16_t rlength;
	uint16_t rdata;

public:
	int parse(string &encoded_message, int &start_index) {
		Utils utils;
		enum MESSAGE_TYPE name_type;
		int idx = start_index;
		uint16_t encoded_type;
		uint8_t temp;

		// aname
		name_type = utils.decode_message_type(encoded_message, start_index);
		if (name_type == TYPE_LABEL) {
			idx = utils.labels_parser(encoded_message, start_index, aname);
		} else {
			idx = utils.pointer_parser(encoded_message, start_index, aname);
		}

		temp = idx;
		// type
		for (; idx < temp + 4; ++idx) {
		   type = type * 16 + ufuncs.hex_to_dig(encoded_message[idx]);
		}
		// class
		temp = idx;
		// type
		for (; idx < temp + 4; ++idx) {
		   type = type * 16 + ufuncs.hex_to_dig(encoded_message[idx]);
		}

		temp = idx;
		// ttl
		for (; idx < temp + 8; ++idx) {
		   type = type * 16 + ufuncs.hex_to_dig(encoded_message[idx]);
		}

		//read rdata
		
	}
	int display() {

	}
};