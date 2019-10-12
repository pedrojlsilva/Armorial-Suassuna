/**
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 * This file is part of the WRBackbone project
 *
 * Copyright (C) 2017 Warthog Robotics
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

// Includes GEARSystem
#include <GEARSystem/gearsystem.hh>

// Selects namespace
using namespace GEARSystem;

int main() {
    // Creates system elements
    WorldMap   wm;
    CommandBus cb;
    CORBAImplementations::Controller controller(&wm, &cb);
    CORBAImplementations::Sensor     sensor(&wm);
    CORBAImplementations::CommandBus commandBus(&cb);

    // Creates and initializes the server
    Server server(&controller, &sensor, &commandBus);
    (void) server.initialize();

    // Starts the server
    server.start();

    return 0;
}
