/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsedMessage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:54:46 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 16:14:14 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>

struct ParsedMessage {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;

	// Struct constructor
	ParsedMessage() : prefix(""), command(""), params() {}
};