#include <stdio.h>
#include <sstream>
#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <math.h>

using namespace curlpp::options;

float getAtMass(std::string str)
{
	int count = 1;
	std::string temp = "";
	std::string mass = "";
	std::string strf = "";
	
	try
	{
		// That's all that is needed to do cleanup of used resources (RAII style).
		curlpp::Cleanup myCleanup;

		// Our request to be sent.
		curlpp::Easy myRequest;
		
		std::string urll = "https://www.lenntech.com/periodic/elements/" + str + ".htm";
		
		std::cout << "\rLoading ..-";
		
		// Set the URL.
		myRequest.setOpt<Url>(urll);
		std::ostringstream os;
		
		// Send request and get a result.
		// By default the result goes to standard output.
		os << myRequest;
		
		strf = os.str();
	}

	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
	}
	
	for( int i = 0; i < (strf.length() - 2) && count <= 4; i++)
	{
		(i % 2 == 0) ? std::cout << "\rLoading . -" : std::cout << "\rLoading ..|";
		temp = temp + strf[i] + strf[i + 1] + strf[i + 2];
		if( temp.compare("<P>") == 0 || temp.compare("<p>") == 0 )
		{
			int k = i + 3;
			while( strf[k + 1] != '/')
			{
				mass = mass + strf[k++];
			}
			
			if( count++ != 4)
			{
				mass = "";
			}
			else
			{
				std::size_t found = mass.find("g.");
				mass = mass.substr(0, found - 1);
			}
		}
		temp = "";
	}
	float mss = std::stof(mass);
	return mss;
}

int main(int argc, char **argv)
{
	int num_elements;
	
	std::cout << "Enter number of elements: ";
	std::cin >> num_elements;
	
	char elements[num_elements][3];
	float masses[num_elements];
	float percentages[num_elements];
	float molar_mass;
	float values[num_elements];
	int emperical_nos[num_elements];
	float empericalWeight = 0;
	int x; // Also callsed 'n'
	float smallest_value = 1000; // Setting to high value, so first iteration will beocme smallest value
	std::cout << "Enter the symbols of the elements(Eg: O, sn, Mg), followed by the percentage" << std::endl;
	for(int i = 0; i < num_elements; i++)
	{
		std::cout  << ">>>";
		std::cin >> elements[i];
		std::cout << "\tPercentage of " << elements[i] << " - ";
		std::cin >> percentages[i];
		elements[i][0] = tolower(elements[i][0]);
		elements[i][1] = tolower(elements[i][1]);
		std::cout << std::endl;
	}
	
	std::string test;
	
	for(int i = 0; i < num_elements; i++)
	{
		test = std::string(elements[i]);
		masses[i] = getAtMass(test);
		elements[i][0] = toupper(elements[i][0]);
		elements[i][1] = tolower(elements[i][1]);
	}
	
	std::cout << "\nEnter molar mass: ";
	std::cin >> molar_mass;
	
	for(int i = 0; i < num_elements; i++)
	{
		values[i] = percentages[i] / masses[i];
		smallest_value = values[i] < smallest_value ? values[i] : smallest_value;
	}
	
	for(int i = 0; i < num_elements; i++)
	{
		emperical_nos[i] = floorf((values[i] / smallest_value) + 0.5f);
		empericalWeight += masses[i] * emperical_nos[i];
	}
	
	std::cout << "Emperical formula" << std::endl;
	for(int i = 0; i < num_elements; i++)
	{
		std::cout << elements[i] << emperical_nos[i];
	}
	
	x = floorf((molar_mass / empericalWeight) + 0.2f); // Rounding up only when decimal is above .8
	
	std::cout << "Molecular formula" << std::endl;
	for(int i = 0; i < num_elements; i++)
	{
		std::cout << elements[i] << (emperical_nos[i] * x);
	}
	
	std::cout << std::endl;
	std::getchar();
	return 0;
}
