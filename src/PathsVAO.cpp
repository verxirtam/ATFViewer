/*
 * =====================================================================================
 *
 *       Filename:  Paths.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月27日 21時53分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "PathsVAO.h"

//TEST
#include <iomanip>

using namespace std;


void PathsVAO::makePathsBuffer(doubleBufferingType& db, TimeSeparation::Position position)
{
	cout << "PathsVAO::makePathsBuffer() start." << endl;
	
	//古いデータを削除する
	//p.clear();
	
	//DBから取得する開始時刻・終了時刻
	time_t time_start = 0;// = this->timeSeparation - this->drawTimeWidth;
	time_t time_end = 0;//   = timeSeparation[time_separation_index + 1] + this->drawTimeWidth;
	
	//時間の区間の開始・終了時刻を取得する
	this->timeSeparation.getIntervalStatus(position, time_start, time_end);
	
	//前後にバッファを持たせる(過去分の軌道も表示するため)
	time_start -= drawTimeWidth;
	time_end += drawTimeWidth;
	
	
	cout << "PathsVAO::makePathsBuffer(): ";
	cout << "time_start = " << time_start;
	cout << ", time_end = " << time_end << endl;
	
	//トラックデータの格納先
	std::vector<Path> p;
	
	//トラックデータの取得
	TrackDataManager tdm;
	tdm.getTrackDataFromDBParallel(p, time_start, time_end, 6);
	
	//トラックデータのVAOへの設定
	this->initVAO(p, db, time_start, time_end);
	
	auto p_max = std::max_element
		(
			p.begin(),
			p.end(),
			[](const Path& p1,const Path& p2)
			{
				return p1.pathPoint.size() < p2.pathPoint.size();
			}
		);
	cout << "max{p[].pathPoint.size()} = " << p_max->pathPoint.size() << endl;
	
	
	cout << "PathsVAO::makePathsBuffer() end." << endl;
}

void PathsVAO::runMakePathsBuffer(doubleBufferingType& db, TimeSeparation::Position position)
{
	cout << "PathsVAO::runMakePathsBuffer() start." << endl;
	
	//スレッドで実行するラムダ式
	//this : メンバ変数を実行するのでコピーキャプチャする
	//db : 書き換えるので参照キャプチャする
	//position : 1次変数なのでコピーキャプチャする
	auto _f = [this, &db, position]{this->makePathsBuffer(db, position);};
	//スレッド起動
	futureMakeBuffer = std::async(std::launch::async, _f);
	
	cout << "PathsVAO::runMakePathsBuffer() end." << endl;
}
void PathsVAO::drawPath(PathPoint& p, time_t now)
{
}
PathPoint PathsVAO::getNowPoint(PathPoint& from, PathPoint& to, time_t time)
{
	PathPoint ret(0.0, 0.0, 0, 0, from.arrival);
	double ratio_from = 1.0 - ((double)(time - from.time))/((double)(to.time - from.time));
	double ratio_to = 1.0 - ratio_from;
	ret.longitude = ratio_from * from.longitude + ratio_to * to.longitude;
	ret.latitude = ratio_from * from.latitude + ratio_to * to.latitude;
	ret.altitude =(int)( ratio_from * ((double)from.altitude) + ratio_to * ((double)to.altitude));
	ret.time = time;
	return ret;
}

void PathsVAO::initPathPoint(time_t time_min, time_t time_max)
{
	//timeSeparationの初期化
	this->timeSeparation = TimeSeparation(time_min, time_max, 2 * 60 * 60);
	
	
	//バッファの取得を別スレッドで実行
	//メインスレッドと合わせて同時取得する
	this->runMakePathsBuffer(*doubleBufferingBuffer, TimeSeparation::Position::next);
	
	//トラックデータ格納用のvector
	//std::vector<Path> paths;
	//メインスレッドで直近で使用するトラックデータを取得
	this->makePathsBuffer(*doubleBufferingCurrent, TimeSeparation::Position::current);
	
	doubleBufferingCurrent->vao.initMain();
	
}

void PathsVAO::initVAO(const std::vector<Path>& path, doubleBufferingType& db, time_t time_start, time_t time_end)
{
	
	using input_type = vaoType::inputType;
	using vertex_type = input_type::value_type;
	input_type input;
	
	std::vector<unsigned int> element;
	std::vector<unsigned int> index_list;
	
	
	unsigned int imax = path.size();
	for(unsigned int i = 0; i < imax; i++)
	{
		unsigned int jmax =  path[i].pathPoint.size();
		//2点以上無ければ軌道が描けないのでスキップ
		if(imax < 2)
		{
			continue;
		}
		//index_listの初期化
		//各Pathの開始インデックスを設定する
		//pastIndex, nowIndexは最初のvertexIndexを設定する
		index_list.push_back(input.size()   + 0);//beginIndex
		index_list.push_back(input.size()   + 2);//pastIndex
		index_list.push_back(input.size()   + 2);//nowIndex
		index_list.push_back(element.size()    );//elementBeginIndex
		{
			//past、nowのベースに使うPathPoint
			const PathPoint& pij = path[i].pathPoint[0];
			
			//past用の頂点
			vertex_type vertex;
			vertex.position = glm::vec3(pij.longitude, pij.latitude, static_cast<float>(pij.altitude));
			vertex.time = static_cast<float>(pij.time - time_start);
			vertex.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			input.push_back(vertex);
			input.push_back(vertex);
			
			//now用の頂点
			input.push_back(vertex);
			input.push_back(vertex);
			
			//1つのTriangleStripで表示するためのダミーの頂点
			element.push_back(element.size());
			element.push_back(element.size());
			
		}
		for(unsigned int j = 0; j < jmax; j++)
		{
			const PathPoint& pij = path[i].pathPoint[j];
			
			//上の点
			float c=((double)pij.altitude)/40000.0f;
			c=c*c;
			vertex_type vertex;
			vertex.position = glm::vec3(pij.longitude, pij.latitude, static_cast<float>(pij.altitude));
			vertex.time =static_cast<float>(pij.time - time_start);
			vertex.color = glm::vec4(c, 0.5f, 1.0f, 1.0f);
			input.push_back(vertex);
			element.push_back(element.size());
			//下の点
			vertex.position.z = std::max(0.0f, vertex.position.z - 1000.0f);
			input.push_back(vertex);
			element.push_back(element.size());
			
		}
		{
			//1つのTriangleStripで表示するためのダミーの頂点
			element.push_back(element.size());
			element.push_back(element.size());
		}
	}
	//index_listの末尾にダミーの項目を追加する
	//最後のPathの末尾を取得するため
	//pastIndex, nowIndexは使用しないのでダミーで0を設定する
	index_list.push_back(  input.size());//beginIndex
	index_list.push_back(             0);//pastIndex
	index_list.push_back(             0);//nowIndex
	index_list.push_back(element.size());//elementBeginIndex
	
	//////////////////////////////////////
	// テスト
	//////////////////////////////////////
	//std::cout << "path.size() = " << path.size() << std::endl;
	//std::cout << "(index_list.size() / 4) - 1 = " << ((index_list.size() / 4) - 1) << std::endl;
	/*
	{
		std::cout << std::setprecision(10);
		
		unsigned int path_index = 10;
		
		for(unsigned int i = path_index; i < path_index + 2; i++)
		{
			for(unsigned int j = 0; j < 4; j++)
			{
				std::cout << index_list[i * 4 + j];
				std::cout << "\t";
			}
			std::cout << std::endl;
		}
		
		unsigned int begin_index = index_list[ path_index      * 4 + 0];
		unsigned int   end_index = index_list[(path_index + 1) * 4 + 0];
		std::cout << "\t{" << std::endl;
		for(unsigned int i = begin_index; i < end_index; i++)
		{
			if((i % 2) == 0)
			{
				std::cout << "\t\t";
			}
			std::cout << input[i].position.x << ", ";
			std::cout << input[i].position.y << ", ";
			std::cout << input[i].position.z << ", ";
			std::cout << input[i].time       << ", ";
			std::cout << input[i].color.r    << ", ";
			std::cout << input[i].color.g    << ", ";
			std::cout << input[i].color.b    << ", ";
			std::cout << input[i].color.a    << ", ";
			std::cout << 0.0f                << ",";
			if((i % 2) == 0)
			{
				std::cout << "\t";
			}
			else
			{
				std::cout << "\t//" << i;
				std::cout << std::endl;
			}
		}
		std::cout << "\t};" << std::endl;
		unsigned int element_begin_index = index_list[ path_index      * 4 + 3];
		unsigned int element_end_index   = index_list[(path_index + 1) * 4 + 3];
		for(unsigned int i = element_begin_index; i < element_end_index; i++)
		{
			std::cout << element[i] << ",\t";
		}
	}
	*/
	//////////////////////////////////////
	
	
	
	//VAOの初期化(裏スレッドで実行可能なGL関数実行部分以外)
	db.vao.initReady(input, element, GL_TRIANGLE_STRIP);

	//index_listのデバイスへの転送
	db.indexListDevice.malloc(index_list.size());
	db.indexListDevice.memcpyHostToDevice(index_list.data());
}


void PathsVAO::resetTime(void)
{
	/*
	for(auto&& p: *currentPaths)
	{
		p.past_time_index = 0;
		p.now_index = 0;
	}
	*/
}

void PathsVAO::updatePastTimeIndex(Path& p, time_t past_time)
{
	int path_size = p.pathPoint.size();
	//past_time_indexを更新する
	//past_time_index = Min{ i | past_time < p[i].time}
	for(int i = p.past_time_index; i < path_size; i++)
	{
		if(past_time < p.pathPoint[i].time)
		{
			p.past_time_index = i;
			break;
		}
	}
	//past_timeが軌道のどの時刻よりも大きい場合はpath_sizeを設定する
	if(p.pathPoint[path_size-1].time <= past_time)
	{
		p.past_time_index = path_size;
	}
}




int PathsVAO::display(time_t now)
{
	
	if(timeSeparation.inNextInterval(now))
	{
		//別スレッドでのデータ取得の完了を待つ
		futureMakeBuffer.wait();
		cout << "futureMakeBuffer.wait() finish." << endl;
		
		//cout << "(*currentPaths).size() = " << (*currentPaths).size() << endl;
		//cout << " pathsBuffer.size() = " <<  pathsBuffer.size() << endl;
		
		//バッファの入れ替え
		//vaoType* dummy = vaoBuffer;
		//vaoBuffer = vaoCurrent;
		//vaoCurrent = dummy;
		doubleBufferingType* dummy = doubleBufferingBuffer;
		doubleBufferingBuffer = doubleBufferingCurrent;
		doubleBufferingCurrent = dummy;
		
		
		//VAOの初期化のGLへの登録部分
		doubleBufferingCurrent->vao.initMain();
		
		//now_indexとpast_time_indexの初期化
		this->resetTime();
		//区間を1つ進める
		timeSeparation.setNextInterval();
		
		//別スレッドで次の区間のトラックデータ取得開始
		this->runMakePathsBuffer(*doubleBufferingBuffer, TimeSeparation::Position::next);
		
	}
	//時刻に応じてデバイス上のPathsをアップデートする
	updateDeviceData(now);
	
	doubleBufferingCurrent->vao.display();
	
	return 0;
	
}
