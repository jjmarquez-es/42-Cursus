/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumarque <jumarque@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:03:54 by jumarque          #+#    #+#             */
/*   Updated: 2026/03/05 20:19:51 by jumarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <stdint.h>

template <typename T>
class Array {
	private:
		T		*_array;
		uint32_t	_size;
	public:
		Array();
		Array(uint32_t numb);
		Array(Array const &oth);
		Array&	operator=(Array const &oth);
		T&	operator[](const uint32_t n);
		uint32_t	size() const;
		~Array();

		class OutOfLimits : public std::out_of_range {
			public:
				OutOfLimits(std::string message) : std::out_of_range(message) {}
		};
};

#include "Array.tpp"