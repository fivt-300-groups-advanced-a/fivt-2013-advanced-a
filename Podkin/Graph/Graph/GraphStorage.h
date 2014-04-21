//
//  GraphStorage.h
//  Graph
//
//  Created by Алексей Подкин on 29.03.14.
//  Copyright (c) 2014 Алексей Подкин. All rights reserved.
//

#ifndef __Graph__GraphStorage__
#define __Graph__GraphStorage__

#include <iostream>
#include "ListOfIncidentVerteces.h"
#include <vector>
#include <algorithm>
#include <memory>

class Graph{
public:
    Graph() {}
    
    explicit Graph(std::vector<std::unique_ptr<ListOfIncidentVerteces> > &_graph)
    {
        graph = std::move(_graph);
    }
    
    const ListOfIncidentVerteces& getIncidentVerteces(int vertex)
    {
        return *graph[vertex];
    }
    
    ~Graph() {}
private:
    std::vector <std::unique_ptr<ListOfIncidentVerteces> > graph;
};

#endif /* defined(__Graph__GraphStorage__) */
