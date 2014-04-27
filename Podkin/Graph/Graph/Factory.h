//
//  Factory.h
//  Graph
//
//  Created by Алексей Подкин on 20.04.14.
//  Copyright (c) 2014 Алексей Подкин. All rights reserved.
//

#ifndef Graph_Factory_h
#define Graph_Factory_h

#include <vector>
#include <memory>
#include "ListOfIncidentVerteces.h"

class Factory
{
public:
    Factory()
    {
        number_of_verteces = 0;
    }
    
    explicit Factory(int cnt)
    {
        number_of_verteces = cnt;
    }
    
    Factory(std::vector<int> to, int cnt)
    {
        incident_verteces = to;
        number_of_verteces = cnt;
    }
    
    void addEdge(int vertex)
    {
        incident_verteces.push_back(vertex);
    }
    
    std::unique_ptr<ListOfIncidentVerteces> getList()
    {
        if ((int)incident_verteces.size() == 1)
        {
            return std::move(std::unique_ptr<ListOfIncidentVerteces>(new FunctionalGraph(incident_verteces[0])));
        }
        if (number_of_verteces < 32 * (int)incident_verteces.size())
        {
            std::vector<bool> bitset(number_of_verteces);
            for (int i = 0; i < (int)incident_verteces.size(); i++)
            {
                bitset[incident_verteces[i]] = true;
            }
            return std::move(std::unique_ptr<ListOfIncidentVerteces>(new BitSetMethod(bitset)));
        }
        return std::move(std::unique_ptr<ListOfIncidentVerteces>(new StandartListMethod(incident_verteces, 0)));
    }
private:
    std::vector<int> incident_verteces;
    int number_of_verteces;
};

#endif
