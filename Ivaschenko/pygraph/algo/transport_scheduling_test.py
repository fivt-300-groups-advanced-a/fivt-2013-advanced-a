__author__ = 'skird'

import unittest
from algo.transport_scheduling import TransportScheduleGraph


class TransportSchedulingIntegrationTest(unittest.TestCase):
    def test_simple_graphs(self):
        graph = TransportScheduleGraph(['Sheremetievo', 'Dolgoprudnaya', 'Novodachnaya',
                                        'Lianozovo', 'Timiryazevskaya', 'Savelovsky terminal'])

        graph.add_route('Dolgoprudnaya', 'Lianozovo', 100, 150)
        graph.add_route('Sheremetievo', 'Savelovsky terminal', 50, 400)  # express train!
        graph.add_route('Lianozovo', 'Timiryazevskaya', 150, 200)
        graph.add_route('Timiryazevskaya', 'Savelovsky terminal', 200, 250)
        graph.add_route('Novodachnaya', 'Lianozovo', 250, 300)
        graph.add_route('Lianozovo', 'Timiryazevskaya', 400, 450)  # waiting for express in Beskudnikovo
        graph.add_route('Timiryazevskaya', 'Savelovsky terminal', 450, 500)

        arrive, path = graph.find_way('Novodachnaya', 'Timiryazevskaya', 200)
        self.assertEqual(arrive, 450)
        self.assertListEqual(path, [('Lianozovo', 250, 300), ('Timiryazevskaya', 400, 450)])

        arrive, path = graph.find_way('Novodachnaya', 'Timiryazevskaya', 250)
        self.assertEqual(arrive, 450)
        self.assertListEqual(path, [('Lianozovo', 250, 300), ('Timiryazevskaya', 400, 450)])

        arrive, path = graph.find_way('Novodachnaya', 'Timiryazevskaya', 251)
        self.assertEqual(arrive, None)
        self.assertEqual(path, None)

        arrive, path = graph.find_way('Dolgoprudnaya', 'Timiryazevskaya', 100)
        self.assertEqual(arrive, 200)
        self.assertListEqual(path, [('Lianozovo', 100, 150), ('Timiryazevskaya', 150, 200)])

        arrive, path = graph.find_way('Dolgoprudnaya', 'Timiryazevskaya', 110)
        self.assertEqual(arrive, None)
        self.assertEqual(path, None)

        graph.add_route('Novodachnaya', 'Dolgoprudnaya', 80, 90)  # on foot ;)

        arrive, path = graph.find_way('Novodachnaya', 'Timiryazevskaya', 80)
        self.assertEqual(arrive, 200)
        self.assertListEqual(path, [('Dolgoprudnaya', 80, 90), ('Lianozovo', 100, 150), ('Timiryazevskaya', 150, 200)])

        arrive, path = graph.find_way('Novodachnaya', 'Timiryazevskaya', 90)
        self.assertEqual(arrive, 450)
        self.assertListEqual(path, [('Lianozovo', 250, 300), ('Timiryazevskaya', 400, 450)])

        self.assertRaises(KeyError, graph.add_route, 'Savelovo', 'Savelovsky Terminal', 100, 500)
        self.assertRaises(KeyError, graph.add_route, 'Novodachnaya', 'Mark', -1, 1)
        self.assertRaises(KeyError, graph.add_route, 'Savelovo', 'Golitsino', 0, 10 ** 5)
