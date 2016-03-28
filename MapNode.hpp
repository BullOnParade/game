#ifndef MAPNODE_HPP
#define MAPNODE_HPP

struct Estremi{
int minX, minY, maxX, maxY;
};

class MapNode
{
    protected:
        int X, Y;
        int building;
        MapNode *N, *S, *E, *W;
        MapNode *next;
        MapNode *LineRight;
        MapNode* FindNode(int x, int y);
        Estremi EstremiMappa();
        void LinkLine(MapNode* pos);
        MapNode* mostLeftLine(int y);
    public:
        MapNode();
        MapNode* GateNord(MapNode* pos);
        MapNode* GateSud(MapNode* pos);
        MapNode* GateEast(MapNode* pos);
        MapNode* GateWest(MapNode* pos);
        MapNode* CreateNord(MapNode* pos);
        MapNode* CreateSud(MapNode* pos);
        MapNode* CreateEast(MapNode* pos);
        MapNode* CreateWest(MapNode* pos);
        void Stampa();

};
#endif // MAPNODE_HPP
