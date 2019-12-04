//
// Created by ThinkPad on 2019/11/30.
//

#ifndef WORKPLACE_LISTDATA_H
#define WORKPLACE_LISTDATA_H

#include <iostream>
#include "Graph.h"
#include "Edge.h"

namespace nGraph {
    template<class EdgeType>
    class ListData {         // element
    public:
        VERTEXE vertex;
        EdgeType weight;
    };

    template<class Elem>
    class ListNode {     // 邻接表每一行的节点
    public:
        Elem element;
        ListNode<Elem> *next;

        ListNode(const Elem &elemval, ListNode<Elem> *nextval = NULL);

        ListNode(ListNode<Elem> *nextval = NULL);
    };

    template<class Elem>
    ListNode<Elem>::ListNode(const Elem &elemval, ListNode<Elem> *nextval) {
        element = elemval;
        next = nextval;
    }

    template<class Elem>
    ListNode<Elem>::ListNode(ListNode<Elem> *nextval) {
        next = nextval;
    }

    template<class Elem>
    class EdgeList {     // 邻接表的一行
    public:
        ListNode<Elem> *head;

        EdgeList() {
            head = new ListNode<Elem>();
        }

        void removeAll();

        ~EdgeList();
    };

    template<class Elem>
    void EdgeList<Elem>::removeAll() {
        ListNode<Elem> *cur;
        while (!head) {
            cur = head;
            head = head->next;
            delete cur;
        }
    }

    template<class Elem>
    EdgeList<Elem>::~EdgeList() {
        removeAll();
    }

    template<class EdgeType>
    class ListGraph : public Graph<EdgeType> {
    private:
        EdgeList<ListData<EdgeType>> *graList;
    public:
        ListGraph(int verticesNum);

        ~ListGraph();

        Edge<EdgeType> FirstEdge(VERTEXE oneVertex);

        Edge<EdgeType> NextEdge(Edge<EdgeType> oneEdge);

        void setEdge(VERTEXE start, VERTEXE end, EdgeType weight);

        void delEdge(VERTEXE start, VERTEXE end);
    };

    template<class EdgeType>
    ListGraph<EdgeType>::ListGraph(int verticesNum):Graph<EdgeType>(verticesNum) {
        graList = new EdgeList<ListData<EdgeType>>[this->vertexNum];
        for (VERTEXE i = 0; i < this->vertexNum; i++) {
            graList[i].head->element.vertex = i;
        }
    }

    template<class EdgeType>
    ListGraph<EdgeType>::~ListGraph() {
        delete[] graList;
    }

    template<class EdgeType>
    Edge<EdgeType> ListGraph<EdgeType>::FirstEdge(VERTEXE oneVertex) {
        Edge<EdgeType> tmpEdge;
        tmpEdge.start = oneVertex;
        ListNode<ListData<EdgeType>> *temp = graList[oneVertex].head;
        if (temp->next != NULL) {
            tmpEdge.end = temp->next->element.vertex;
            tmpEdge.weight = temp->next->element.weight;
        }
        return tmpEdge;
    }

    template<class EdgeType>
    Edge<EdgeType> ListGraph<EdgeType>::NextEdge(Edge<EdgeType> oneEdge) {
        Edge<EdgeType> tmpEdge;
        tmpEdge.start = oneEdge.start;
        ListNode<ListData<EdgeType>> *temp = graList[oneEdge.start].head;
        while (temp->next != NULL && temp->next->element.vertex <= oneEdge.end) {
            temp = temp->next;
        }

        if (temp->next != NULL) {
            tmpEdge.end = temp->next->element.vertex;
            tmpEdge.weight = temp->next->element.weight;
            return tmpEdge;
        }
        return tmpEdge;
    }

    template<class EdgeType>
    void ListGraph<EdgeType>::setEdge(VERTEXE start, VERTEXE end, EdgeType weight) {
        ListNode<ListData<EdgeType>> *temp = graList[start].head;
        while (temp->next != NULL && temp->next->element.vertex < end) {
            temp = temp->next;
        }
        if (temp->next == NULL) {
            temp->next = new ListNode<ListData<EdgeType>>();
            temp->next->element.vertex = end;
            temp->next->element.weight = weight;
            this->edgeNum++;
            return;
        }
        if (temp->next->element.vertex == end) {
            temp->next->element.weight = weight;
            return;
        }
        if (temp->next->element.vertex > end) {
            ListNode<ListData<EdgeType>> *other = temp->next;
            temp->next = new ListNode<ListData<EdgeType>>();
            temp->next->element.vertex = end;
            temp->next->element.weight = weight;
            temp->next->next = other;
            this->edgeNum++;
        }
    }

    template<class EdgeType>
    void ListGraph<EdgeType>::delEdge(VERTEXE start, VERTEXE end) {
        ListNode<ListData<EdgeType>> *temp = graList[start].head;
        while (temp->next != NULL && temp->next->element.vertex < end) {
            temp = temp->next;
        }
        if (temp->next == NULL) return;
        if (temp->next->element.vertex == end) {
            ListNode<ListData<EdgeType>> *other = temp->next->next;
            delete temp->next;
            temp->next = other;
            this->edgeNum--;
        }
    }
}


#endif //WORKPLACE_LISTDATA_H