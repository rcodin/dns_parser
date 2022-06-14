#include "utils.h"

#include <vector>
#include <string>

class DNS_Parser {
private:
	std::string encoded_message;
	std::vector<Section*> sections;
public:
	DNS_Parser() {

	}
	int parse(std::string &encoded_message) {
		this->encoded_message = encoded_message;
		int idx = 0;
		// parse question section
		Section *header = new Header();

		idx = header.parse(encoded_message, idx);
		sections.push_back(header);

		// get number fof question sections and answer sections
		uint16_t num_question_sections = header.num_qs();
		uint16_t num_answer_sections = header.num_an();
		uint16_t num_authority_sections = header.num_ns();;
		uint16_t num_additional_sections = header.num_ar();;
		// parse that many sections
		for (int idx = 0; idx < num_question_sections; ++idx) {
			Section sec = new QuestionSection();
			idx = sec.parse(encoded_message, idx);
			sections.push_back(sec);
		}

		for (int idx = 0; idx < num_answer_sections; ++idx) {
			Section sec = new AnswerSection();
			idx = sec.parse(encoded_message, idx);
			sections.push_back(sec);
		}

		for (int idx = 0; idx < num_authority_sections; ++idx) {
			Section sec = new AnswerSection();
			idx = sec.parse(encoded_message, idx);
			sections.push_back(sec);
		}

		for (int idx = 0; idx < num_additional_sections; ++idx) {
			Section sec = new AnswerSection();
			idx = sec.parse(encoded_message, idx);
			sections.push_back(sec);
		}

	}
	int display() {
		// call sections
		for (Section &sec: sections)
			cout<<sec<<endl;
	}
};

int main () {
	string input = R""""(a01d81800001000100000000
				076578616d706c6503636f6d0000010001
				c00c0001000100001bbc00045db8d822)"""";
	/*
	 * TODO: Have CLI options saying of taking strings directly and via a
	 * file
	 */

}

// Example:
// a01d81800001000100000000

// 076578616d706c6503636f6d0000010001

// c00c0001000100001bbc00045db8d822