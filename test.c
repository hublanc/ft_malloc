/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 14:11:13 by hublanc           #+#    #+#             */
/*   Updated: 2018/09/04 16:19:54 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>


int main (int argc, char **argv){
	int mainRet = 0;
	int pagesizeRet = getpagesize();
	printf("GetPageSize return: %d \n", pagesizeRet);
	return (mainRet);
}
