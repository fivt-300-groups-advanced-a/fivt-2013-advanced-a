__author__ = 'Алексей'

import copy
import random


class Ordinal:
    def __init__(self, lst):
        """
        Ordinal is a class for ordinal numbers, which are less than w^w
        It is a non-empty list of Ordinal Monomials.
        Ordinal Monomials are tuples (degree, index)
        For example list [(6,2), (2,1), (0,10)] represents ordinal w^6 * 2 + w^2 * 1 + 10
        :param lst: list of tuples of two elements for current ordinal number.
                     Must be ordered by degree decs
        """
        self.list = lst

    def __eq__(self, other):
        return self.list == other.list


def ordinal_sum(a, b):
    """
    Summarise two ordinals
    :param a: First ordinal
    :param b: Second ordinal
    :return: Ordinal a + b
    """
    result = copy.deepcopy(a)
    while len(result.list) > 0 and b.list[0][0] > result.list[-1][0]:
        result.list.pop()
    if len(result.list) > 0 and result.list[-1][0] == b.list[0][0]:
        result.list[-1] = (result.list[-1][0], result.list[-1][1] + b.list[0][1])
        result.list.extend(b.list[1:])
    else:
        result.list.extend(b.list)
    if len(result.list) > 1 and result.list[-1] == (0, 0):
        result.list.pop()
    return result


def ordinal_cmp(a, b):
    """
    Compares two ordinals
    Returns true if a < b
    False otherwise
    :param a: ordinal a
    :param b: ordinal b
    """
    return a.list < b.list


def get_random_ordinal(max_value):
    result = []
    for i in range(max_value):
        result.append((i, random.randint(1, max_value)))
    result.reverse()
    return Ordinal(result)