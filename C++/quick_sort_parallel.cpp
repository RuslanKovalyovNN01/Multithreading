#include <algorithm>
#include <iostream>
#include <future>
#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include <list>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}
	std::list<T> result;
	result.splice(result.begin(), input, input.begin()); 
	T const& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(),
		[&](T const& t) {return t < pivot; }); 
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(),
	divide_point); 
	auto new_lower(
			sequential_quick_sort(std::move(lower_part))); 
	auto new_higher(
			sequential_quick_sort(std::move(input))); 
	result.splice(result.end(), new_higher); 
	result.splice(result.begin(), new_lower); 
	return result;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T const& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(),
		[&](T const& t) {return t < pivot; });
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(),
		divide_point);
	std::future<std::list<T> > new_lower(
		std::async(&parallel_quick_sort<T>, std::move(lower_part)));
	auto new_higher(
		parallel_quick_sort(std::move(input))); 
	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower.get());
	return result;
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);

	const int N = 10000;
	std::list<int> data;
	for (int i = 0; i < N; ++i)
		data.push_back(dist(gen));

	std::list<int> data_copy = data;

	auto start_seq = std::chrono::high_resolution_clock::now();
	auto seq_sorted = sequential_quick_sort(data_copy);
	auto end_seq = std::chrono::high_resolution_clock::now();
	auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq).count();

	auto start_par = std::chrono::high_resolution_clock::now();
	auto par_sorted = parallel_quick_sort(data);
	auto end_par = std::chrono::high_resolution_clock::now();
	auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par).count();

	if (seq_sorted == par_sorted)
		std::cout << "Success: both versions produce the same result.\n";
	else
		std::cout << "Error: results differ!\n";

	std::cout << "Sequential sort time: " << seq_time << " ms\n";
	std::cout << "Parallel sort time: " << par_time << " ms\n";

	return 0;
}