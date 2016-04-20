#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include "anro1/mapMessage.h"
#include <cstdlib>
#include <visualization_msgs/Marker.h>
using namespace std;


class MapOfCity{



public:

    visualization_msgs::Marker marker1, marker2, marker3, tree, leaves, tree1, leaves1, tree2, leaves2;
    ros::NodeHandle n;
    ros::Publisher marker_pub;

    MapOfCity()
    {
        sendCrossroadsInfo();
        configureRviz();
        sendMapVisualization();
    };

    void configureRviz()
    {
        marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
        ros::Rate loop_rate(100);
        while(marker_pub.getNumSubscribers() < 1 )
        {
            loop_rate.sleep();
        }


    }

    // stworzy drzewa , budynki itp
    void createBuildings()
    {

        uint32_t shape = visualization_msgs::Marker::CUBE;
        marker1.header.frame_id = "/my_frame";
        marker1.header.stamp = ros::Time::now();
        marker1.type = shape;
        marker1.ns = "MapOfCity";
        marker1.id = 4;
        marker1.action = visualization_msgs::Marker::ADD;
        marker1.pose.position.x = 75;
        marker1.pose.position.y = 75;
        marker1.pose.position.z = 25;
        marker1.pose.orientation.x = 0.0;
        marker1.pose.orientation.y = 0.0;
        marker1.pose.orientation.z = 0.0;
        marker1.pose.orientation.w = 1.0;
        marker1.scale.x = 5.0;
        marker1.scale.y = 5.0;
        marker1.scale.z = 25.0;
        marker1.color.r = 0.5f;
        marker1.color.g = 0.0f;
        marker1.color.b = 0.0f;
        marker1.color.a = 1.0;
        marker1.lifetime = ros::Duration();


        marker2.header.frame_id = "/my_frame";
        marker2.header.stamp = ros::Time::now();
        marker2.type = shape;
        marker2.ns = "MapOfCity";
        marker2.id = 5;
        marker2.action = visualization_msgs::Marker::ADD;
        marker2.pose.position.x = 75;
        marker2.pose.position.y = 75;
        marker2.pose.position.z = 12.5;
        marker2.pose.orientation.x = 0.0;
        marker2.pose.orientation.y = 0.0;
        marker2.pose.orientation.z = 0.0;
        marker2.pose.orientation.w = 1.0;
        marker2.scale.x = 10.0;
        marker2.scale.y = 10.0;
        marker2.scale.z = 25.0;
        marker2.color.r = 0.0f;
        marker2.color.g = 0.0f;
        marker2.color.b = 0.0f;
        marker2.color.a = 2.0;
        marker2.lifetime = ros::Duration();

        shape = visualization_msgs::Marker::SPHERE;
        marker3.header.frame_id = "/my_frame";
        marker3.header.stamp = ros::Time::now();
        marker3.type = shape;
        marker3.ns = "MapOfCity";
        marker3.id = 6;
        marker3.action = visualization_msgs::Marker::ADD;
        marker3.pose.position.x = 75;
        marker3.pose.position.y = 75;
        marker3.pose.position.z = 55;
        marker3.pose.orientation.x = 70.0;
        marker3.pose.orientation.y = 70.0;
        marker3.pose.orientation.z = 20;
        marker3.pose.orientation.w = 1.0;
        marker3.scale.x = 10.0;
        marker3.scale.y = 10.0;
        marker3.scale.z = 40.0;
        marker3.color.r = 1.0f;
        marker3.color.g = 1.0f;
        marker3.color.b = 1.0f;
        marker3.color.a = 1.0;
        marker3.lifetime = ros::Duration();

        shape = visualization_msgs::Marker::CUBE;
        tree.header.frame_id = "/my_frame";
        tree.header.stamp = ros::Time::now();
        tree.type = shape;
        tree.ns = "MapOfCity";
        tree.id = 7;
        tree.action = visualization_msgs::Marker::ADD;
        tree.pose.position.x = 70;
        tree.pose.position.y = 35;
        tree.pose.position.z = 5;
        tree.pose.orientation.x = 0.0;
        tree.pose.orientation.y = 0.0;
        tree.pose.orientation.z = 0;
        tree.pose.orientation.w = 1.0;
        tree.scale.x = 2.0;
        tree.scale.y = 2.0;
        tree.scale.z = 10.0;
        tree.color.r = 0.5f;
        tree.color.g = 0.25f;
        tree.color.b = 0.07f;
        tree.color.a = 1.0;
        tree.lifetime = ros::Duration();

        shape = visualization_msgs::Marker::SPHERE;
        leaves.header.frame_id = "/my_frame";
        leaves.header.stamp = ros::Time::now();
        leaves.type = shape;
        leaves.ns = "MapOfCity";
        leaves.id = 8;
        leaves.action = visualization_msgs::Marker::ADD;
        leaves.pose.position.x = 70;
        leaves.pose.position.y = 35;
        leaves.pose.position.z = 15;
        leaves.pose.orientation.x = 0.0;
        leaves.pose.orientation.y = 0.0;
        leaves.pose.orientation.z = 0;
        leaves.pose.orientation.w = 1.0;
        leaves.scale.x = 15.0;
        leaves.scale.y = 15.0;
        leaves.scale.z = 10.0;
        leaves.color.r = 0.1f;
        leaves.color.g = 0.8f;
        leaves.color.b = 0.1f;
        leaves.color.a = 1.0;
        leaves.lifetime = ros::Duration();

        tree1 = tree;
        tree1.id = 19;
        tree1.pose.position.x = 85 ;
        tree1.pose.position.y = 25;


        leaves1 = leaves;
        leaves1.id = 10;
        leaves1.pose.position.x = 85 ;
        leaves1.pose.position.y = 25;

        tree2 = tree1;
        tree2.id = 11;
        tree2.pose.position.x = 65 ;
        tree2.pose.position.y = 15;

        leaves2 = leaves;
        leaves2.id = 12;
        leaves2.pose.position.x = 65 ;
        leaves2.pose.position.y = 15;

    }
    void sendMapVisualization()
    {

        // funkcja do drogi


        visualization_msgs::Marker  points, line_strip, line_list, cubes;



        // stworz budynki i drzewa
        createBuildings();

        // wyslij do rviza
        marker_pub.publish(marker1);
        marker_pub.publish(marker2);
        marker_pub.publish(marker3);
        marker_pub.publish(tree);
        marker_pub.publish(leaves);
        marker_pub.publish(tree1);
        marker_pub.publish(leaves1);
        marker_pub.publish(tree2);
        marker_pub.publish(leaves2);

        cubes.header.frame_id = "/my_frame";
        cubes.header.stamp = ros::Time::now();
        cubes.ns = "MapOfCity";
        cubes.action = visualization_msgs::Marker::ADD;
        cubes.pose.orientation.w = 1.0;


        cubes.id = 0;
        cubes.type = visualization_msgs::Marker::CUBE_LIST;


        cubes.scale.x = 10;
        cubes.scale.y = 10;
        cubes.scale.z = 0.2;
        cubes.color.r = 1.0f;
        cubes.color.g = 1.0f;
        cubes.color.b = 1.0f;
        cubes.color.a = 1.0f;

        points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/my_frame";
        points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
        points.ns = line_strip.ns = line_list.ns = "MapOfCity";
        points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
        points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;



        points.id = 1;
        line_strip.id = 2;
        line_list.id = 3;



        points.type = visualization_msgs::Marker::POINTS;
        line_strip.type = visualization_msgs::Marker::LINE_STRIP;
        line_list.type = visualization_msgs::Marker::LINE_LIST;



        // POINTS markers use x and y scale for width/height respectively
        points.scale.x = 0.2;
        points.scale.y = 0.2;

        // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
        line_strip.scale.x = 0.1;
        line_list.scale.x = 0.1;



        // Points are green
        points.color.b = 1.0f;
        points.color.a = 1.0;

        // Line strip is blue
        line_strip.color.b = 1.0;
        line_strip.color.a = 1.0;



        // Create the vertices for the points and lines
        for (uint32_t i = 0; i < 150; ++i)
        {

            float y = 50;
            float z = 0.00;
            geometry_msgs::Point p;
            p.x = (int32_t)i;
            p.y = y;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);


        for (uint32_t i = 50; i < 100; ++i)
        {

            float x = 150;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = x;
            p.y = (int32_t)i;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);

        for (uint32_t i = 150; i > 100; --i)
        {

            float y = 100;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = (int32_t)i;
            p.y = y;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);

        for (uint32_t i = 100; i > 0; --i)
        {

            float x = 100;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = x;
            p.y = (int32_t)i;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);

        for (uint32_t i = 100; i > 50; --i)
        {

            float y = 0;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = (int32_t)i;
            p.y = y;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);


        for (uint32_t i = 0; i < 100; ++i)
        {

            float x = 50;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = x;
            p.y = (int32_t)i;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);

        for (uint32_t i = 50; i > 0; --i)
        {

            float y = 100;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = (int32_t)i;
            p.y = y;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);

        for (uint32_t i = 100; i > 50; --i)
        {

            float x = 0;
            float z = 0.000;
            geometry_msgs::Point p;
            p.x = 0;
            p.y = (int32_t)i;
            p.z = z;

            points.points.push_back(p);
            line_strip.points.push_back(p);
            cubes.points.push_back(p);

        }


        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        marker_pub.publish(cubes);



    }

    // metoda sendCrossroadsInfo wysyła w topicu map_info message mapMessage zawierajace pola okreslajace polozenie oraz typ skrzyzownia/zakretu
    void sendCrossroadsInfo()
    {
        //konfiguracja polaczenia
        ros::NodeHandle n;
        ros::Publisher map_info = n.advertise<anro1::mapMessage>("map_info", 1000);
        anro1::mapMessage crossroad1;
        anro1::mapMessage turning;

        //petla czeka okolo 10sek na listener
        ros::Rate loop_rate(10);

        ROS_INFO("I heard: ");
        // wysylanie informacji o polozeniu skrzyzowan

        // czekaj poki nie bedzie trzech subskrypji (lights, turns, model)
        while (map_info.getNumSubscribers() < 2 )
        {

            ROS_WARN_ONCE("Please create a subscriber to the marker");
            loop_rate.sleep();
        }

        int counter=1;
        ROS_WARN_ONCE("Subscribed");
        while(counter>0)
        {
            counter--;
            ROS_INFO("I heard: ");

            crossroad1.x = 50 ;
            crossroad1.y = 50 ;
            crossroad1.type = "fourLanes";
            map_info.publish(crossroad1);

            crossroad1.x = 100 ;
            crossroad1.y = 50 ;
            crossroad1.type = "fourLanes";
            map_info.publish(crossroad1);


            turning.x = 150 ;
            turning.y = 50 ;
            turning.type = "turningUpLeft";
            map_info.publish(turning);

            turning.x = 150 ;
            turning.y = 100 ;
            turning.type = "turningDownLeft";
            map_info.publish(turning);

            turning.x = 100 ;
            turning.y = 100 ;
            turning.type = "turningDownRight";
            map_info.publish(turning);

            turning.x = 100 ;
            turning.y = 0 ;
            turning.type = "turningUpLeft";
            map_info.publish(turning);

            turning.x = 50 ;
            turning.y = 0 ;
            turning.type = "turningUpRight";
            map_info.publish(turning);

            turning.x = 50 ;
            turning.y = 100 ;
            turning.type = "turningDownLeft";
            map_info.publish(turning);

            turning.x = 0 ;
            turning.y = 100 ;
            turning.type = "turningDownRight";
            map_info.publish(turning);

            turning.x = 0 ;
            turning.y = 50 ;
            turning.type = "turningUpRight";
            map_info.publish(turning);
            loop_rate.sleep();
        }

    }

};



int main(int argc, char** argv)
{
    ros::init(argc, argv, "MapOfCity");
    new MapOfCity();
    ROS_INFO("Exiting..");
    return 0;
}
