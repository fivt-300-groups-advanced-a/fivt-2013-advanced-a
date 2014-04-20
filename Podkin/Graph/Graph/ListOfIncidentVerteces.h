//
//  ListOfIncidentVerteces.h
//  Graph
//
//  Created by Алексей Подкин on 29.03.14.
//  Copyright (c) 2014 Алексей Подкин. All rights reserved.
//

#ifndef __Graph__ListOfIncidentVerteces__
#define __Graph__ListOfIncidentVerteces__

#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

class Iterator
{
public:
    virtual int getIncidentVertex() = 0;
    virtual bool nextVertexWithEdge() = 0;
    virtual ~Iterator() {}
};

class ListOfIncidentVerteces
{
public:
    ListOfIncidentVerteces() {}
    virtual std::unique_ptr<Iterator> getIterator() const = 0;
    virtual bool isConnected(int vertex) const = 0;
    virtual ~ListOfIncidentVerteces() {}
};

class BitSetIterator : public Iterator
{
public:
    BitSetIterator(const ListOfIncidentVerteces *list, int _current, int _last)
    {
        current_vertex = _current;
        last_vertex = _last;
        bit_set = list;
    }
    
    int getIncidentVertex() override
    {
        return current_vertex;
    }
    
    bool nextVertexWithEdge() override
    {
        current_vertex++;
        while (current_vertex <= last_vertex && !bit_set->isConnected(current_vertex))
        {
            current_vertex++;
        }
        if (current_vertex > last_vertex)
        {
            return false;
        }
        return true;
    }
    
    virtual ~BitSetIterator() {}
private:
    const ListOfIncidentVerteces *bit_set;
    int current_vertex, last_vertex;
};

template <class UserIterator>
class StdIterator : public Iterator
{
public:
    StdIterator (UserIterator _first, UserIterator _last)
    {
        current = _first;
        last = _last;
    }
    
    virtual ~StdIterator() {}
    
    int getIncidentVertex() override
    {
        return *current;
    }
    
    bool nextVertexWithEdge() override
    {
        if (current == last)
        {
            return false;
        }
        current++;
        return true;
    }
private:
    UserIterator current;
    UserIterator last;
};

class FunctionalGraph : public ListOfIncidentVerteces
{
public:
    explicit FunctionalGraph(int _to)
    {
        to = _to;
    }
    
    bool isConnected(int vertex) const override
    {
        return to == vertex;
    }
    
    std::unique_ptr<Iterator> getIterator() const override {
        return std::move(std::unique_ptr<Iterator>(new BitSetIterator(this, to, to)));
    }
    
    virtual ~FunctionalGraph() {}
private:
    int to;
};

class BitSetMethod : public ListOfIncidentVerteces
{
public:
    explicit BitSetMethod(const std::vector<int> &list, int number_of_verteces)
    {
        incident_verteces.resize(number_of_verteces, false);
        for (int i = 0; i < list.size(); i++)
        {
            incident_verteces[list[i]] = true;
        }
    }
    
    explicit BitSetMethod(const std::vector<bool> &list)
    {
        incident_verteces = list;
    }
    
    bool isConnected(int vertex) const override
    {
        if (vertex < 0 || vertex >= incident_verteces.size())
        {
            return false;
        }
        else{
            return incident_verteces[vertex];
        }
    }
    
    std::unique_ptr<Iterator> getIterator() const override
    {
        return std::move(std::unique_ptr<Iterator>(new BitSetIterator(this, 0, (int)incident_verteces.size() - 1)));
    }
    
    virtual ~BitSetMethod() {}
private:
    std::vector<bool> incident_verteces;
};

class StandartListMethod : public ListOfIncidentVerteces
{
public:
    StandartListMethod(const std::vector<int> &list, bool sorted)
    {
        incident_verteces = list;
        if (!sorted)
        {
            std::sort(incident_verteces.begin(), incident_verteces.end());
        }
    }
    
    bool isConnected(int vertex) const override
    {
        return std::binary_search(incident_verteces.begin(), incident_verteces.end(), vertex);
    }
    
    std::unique_ptr<Iterator> getIterator() const override
    {
        return std::move(std::unique_ptr<Iterator>(new StdIterator<std::vector<int>::const_iterator>(incident_verteces.begin(), incident_verteces.end())));
    }
    
    virtual ~StandartListMethod() {}
private:
    std::vector<int> incident_verteces;
};
#endif /* defined(__Graph__ListOfIncidentVerteces__) */
