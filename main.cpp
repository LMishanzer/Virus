#include <iostream>
#include <list>

using namespace std;

struct path {
    int x;
    int y;
};

struct Planet{
    int number;
    int distFromStart;
    bool isInfected;
    list<Planet*> neighbors;

    Planet(){
        number = 0;
        distFromStart = INT32_MAX;
        isInfected = false;
    }
};

class Graph{
public:
    Graph(path* paths, int planetsCount, int pathsCount, int start, int terminate,
          const int* infectedPlanets, int infectedPlanetsCount, int pathAliveCount){
        planets = new Planet[planetsCount];
        startPlanet = &planets[start];
        terminatePlanet = &planets[terminate];
        this->pathAliveCount = pathAliveCount;

        for (int i = 0; i < pathsCount; i++){
            planets[paths[i].x].neighbors.push_back(&planets[paths[i].y]);
            planets[paths[i].y].neighbors.push_back(&planets[paths[i].x]);
        }

        for (int i = 0; i < planetsCount; i++){
            planets[i].number = i;
        }

        for (int i = 0; i < infectedPlanetsCount; i++){
            planets[infectedPlanets[i]].isInfected = true;
        }
    }

    ~Graph(){
        delete [] planets;
    }

    list<Planet*> CreateRoute(){
        startPlanet->distFromStart = 0;
        CreatingRoute(startPlanet, 0);

        if (terminatePlanet->distFromStart == INT32_MAX) {
            route.clear();
            return route;
        }

        FindRoute(terminatePlanet);

        return route;
    }

private:
    void CreatingRoute(Planet* current, int pathInfected){
        if ((pathInfected >= pathAliveCount && pathAliveCount > 0)
        || (current->isInfected && pathAliveCount == 0))
            return;

        if (current->isInfected && pathInfected == 0)
            pathInfected = 1;
        else if (pathInfected > 0)
            pathInfected++;

        for(auto neighbor: current->neighbors){
            if (neighbor->distFromStart > current->distFromStart + 1){
                neighbor->distFromStart = current->distFromStart + 1;
                CreatingRoute(neighbor, pathInfected);
            }
        }
    }

    void FindRoute(Planet* current){
        route.push_front(current);
        for (auto neighbor: current->neighbors){
            if (neighbor->distFromStart == current->distFromStart - 1){
                FindRoute(neighbor);
                break;
            }
        }
    }

    Planet* startPlanet;
    Planet* terminatePlanet;
    Planet* planets;
    int pathAliveCount;
    list<Planet*> route;
};

void readInput(int& plantCount,
               int& pathsCount,
               int& startPlanet,
               int& terminatePlanet,
               int& pathAliveCount,
               int& infectedPlanetsCount,
               int*& infectedPlanets,
               int& planetsWithVaccineCount,
               int*& planetsWithVaccine,
               path*& paths)
{
    cin >> plantCount >> pathsCount;
    cin >> startPlanet >> terminatePlanet >> pathAliveCount;
    cin >> infectedPlanetsCount;
    infectedPlanets = new int[infectedPlanetsCount];

    for (int i = 0; i < infectedPlanetsCount; i++){
        cin >> infectedPlanets[i];
    }

    cin >> planetsWithVaccineCount;
    planetsWithVaccine = new int[planetsWithVaccineCount];

    for (int i = 0; i < planetsWithVaccineCount; i++){
        cin >> planetsWithVaccine[i];
    }

    paths = new path[pathsCount];

    for (int i = 0; i < pathsCount; i++)
    {
        cin >> paths[i].x >> paths[i].y;
    }
}

int main() {
    int planetsCount, pathsCount, startPlanet, terminatePlanet, pathAliveCount,
    infectedPlanetsCount, planetsWithVaccineCount;

    int* infectedPlanets = nullptr;
    int* planetsWithVaccines = nullptr;
    path* paths = nullptr;

    readInput(planetsCount, pathsCount, startPlanet, terminatePlanet, pathAliveCount,
              infectedPlanetsCount, infectedPlanets, planetsWithVaccineCount,
              planetsWithVaccines, paths);

    auto graph = new Graph(paths, planetsCount, pathsCount, startPlanet, terminatePlanet,
                           infectedPlanets, infectedPlanetsCount, pathAliveCount);
    delete [] paths;
    delete [] infectedPlanets;
    delete [] planetsWithVaccines;

    auto result = graph->CreateRoute();

    if (result.empty()){
        cout << -1;
    }
    else {
        for (auto planet: result) {
            cout << planet->number << ' ';
        }
    }
    cout << endl;

    delete graph;

    return 0;
}
