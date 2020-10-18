#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct Planet{
    int number;
    int distFromStart;
    int from;
    bool isInfected;
    bool isHealing;
    bool wasShipInfected;
    vector<int> neighbors;
    int pathInfected;

    Planet(){
        number = 0;
        distFromStart = INT32_MAX;
        isInfected = false;
        pathInfected = 0;
        from = 0;
        wasShipInfected = false;
        isHealing = false;
    }
};

class Graph{
public:
    Graph(Planet* planets, int start, int terminate, int pathsAliveCount){
        this->planets = planets;
        startPlanet = start;
        terminatePlanet = terminate;
        this->pathAliveCount = pathsAliveCount;
    }

    ~Graph(){
        delete [] planets;
    }

    stack<int> CreateRoute(){
        planets[startPlanet].distFromStart = 0;
        CreatingRoute();

        if (planets[terminatePlanet].distFromStart == INT32_MAX) {
            return stack<int>();
        }

        FindRoute();

        return route;
    }

private:
    void CreatingRoute(){
        stack<int> planetStack;
        planetStack.emplace(startPlanet);

        while(!planetStack.empty()) {
            int current = planetStack.top();
            planetStack.pop();

            if (current == terminatePlanet)
                break;

            if (planets[current].isInfected) {
                planets[current].wasShipInfected = true;
            }
            if (planets[current].wasShipInfected) {
                if (planets[current].pathInfected >= pathAliveCount)
                    continue;
                planets[current].pathInfected++;
            }

            for (auto neighbor: planets[current].neighbors) {
                if (planets[neighbor].distFromStart > planets[current].distFromStart + 1
                || planets[neighbor].pathInfected > planets[current].pathInfected
                || (planets[neighbor].wasShipInfected && !planets[current].wasShipInfected)) {
                    planets[neighbor].distFromStart = planets[current].distFromStart + 1;
                    planets[neighbor].pathInfected = planets[current].pathInfected;
                    planets[neighbor].from = current;
                    planets[neighbor].wasShipInfected = planets[current].wasShipInfected;
                    planetStack.push(neighbor);
                }
            }
        }
    }

    void FindRoute(){
        int current = terminatePlanet;
        route.push(current);

        while (planets[current].number != planets[startPlanet].number) {
            current = planets[current].from;
            route.push(current);
        }
    }

    int startPlanet;
    int terminatePlanet;
    Planet* planets;
    int pathAliveCount;
    stack<int> route;
};

void readInput(Planet*& planets,
               int& startPlanet,
               int& terminatePlanet,
               int& pathAliveCount)
{
    int pathsCount, infectedPlanetsCount, planetsWithVaccineCount, planetsCount;
    cin >> planetsCount >> pathsCount;
    cin >> startPlanet >> terminatePlanet >> pathAliveCount;
    cin >> infectedPlanetsCount;

    planets = new Planet[planetsCount];

    for (int i = 0; i < planetsCount; i++){
        planets[i].number = i;
    }

    for (int i = 0; i < infectedPlanetsCount; i++){
        int infectedPlanet;
        cin >> infectedPlanet;
        planets[infectedPlanet].isInfected = true;
    }

    cin >> planetsWithVaccineCount;

    for (int i = 0; i < planetsWithVaccineCount; i++){
        int planetWithVaccine;
        cin >> planetWithVaccine;
        planets[planetWithVaccine].isHealing = true;
    }

    for (int i = 0; i < pathsCount; i++)
    {
        int x, y;
        cin >> x >> y;
        planets[x].neighbors.push_back(y);
        planets[y].neighbors.push_back(x);
    }
}

int main() {
    Planet* planets = nullptr;

    int pathsAliveCount, startPlanet, terminatePlanet;

    readInput(planets, startPlanet, terminatePlanet, pathsAliveCount);

   auto graph = new Graph(planets, startPlanet, terminatePlanet, pathsAliveCount);

   auto result = graph->CreateRoute();

   if (result.empty()){
       cout << -1;
   }
   else {
       while (!result.empty()) {
           cout << result.top() << ' ';
           result.pop();
       }
   }
   cout << endl;

   delete graph;

//     delete [] planets;

    return 0;
}
