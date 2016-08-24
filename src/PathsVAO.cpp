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

using namespace std;


void PathsVAO::makePathsBuffer(std::vector<Path>& p, TimeSeparation::Position position)
{
	cout << "PathsVAO::makePathsBuffer() start." << endl;
	
	//古いデータを削除する
	p.clear();
	//DBから取得する開始時刻・終了時刻
	time_t time_start = 0;// = this->timeSeparation - this->drawTimeWidth;
	time_t time_end = 0;//   = timeSeparation[time_separation_index + 1] + this->drawTimeWidth;
	
	//時間の区間の開始・終了時刻を取得する
	this->timeSeparation.getIntervalStatus(position, time_start, time_end);
	
	//前後にバッファを持たせる(過去分の起動も表示するため)
	time_start -= drawTimeWidth;
	time_end += drawTimeWidth;
	
	
	cout << "PathsVAO::makePathsBuffer(): ";
	cout << "time_start = " << time_start;
	cout << ", time_end = " << time_end << endl;
	
	
	//トラックデータの取得
	TrackDataManager tdm;
	tdm.getTrackDataFromDBParallel(p, time_start, time_end, 6);
	
	cout << "PathsVAO::makePathsBuffer() end." << endl;
}

void PathsVAO::runMakePathsBuffer(std::vector<Path>& p, TimeSeparation::Position position)
{
	cout << "PathsVAO::runMakePathsBuffer() start." << endl;
	
	//スレッドで実行するラムダ式
	//this : メンバ変数を実行するのでコピーキャプチャする
	//p : 書き換えるので参照キャプチャする
	//position : 1次変数なのでコピーキャプチャする
	auto _f = [this,&p,position]{this->makePathsBuffer(p, position);};
	//スレッド起動
	futureMakeBuffer = std::async(std::launch::async, _f);
	
	cout << "PathsVAO::runMakePathsBuffer() end." << endl;
}
void PathsVAO::drawPath(PathPoint& p, time_t now)
{
	/*
	//航空機の高度に応じて色を設定する
	//帯の地面に接する箇所はアルファを0にする（完全に透明にする）
	double c=((double)p.altitude)/40000.0;
	double alpha = 1.0 - (((double)(now - p.time)) / ((double)drawTimeWidth));
	c=c*c;
	//glColor4d(c, 0.5, 1.0 - c, alpha * (1.0 - c));
	glColor4d(c, 0.5, 1.0 - c, alpha * 0.5);
	glVertex3d(p.longitude, p.latitude, p.altitude);
	glColor4d(c, 0.5, 1.0 - c, alpha * 0.1);
	glVertex3d(p.longitude, p.latitude, 0.0);
	*/
	
	//目的空港の文字列に応じて配色する
	double alpha = 1.0 - (((double)(now - p.time)) / ((double)drawTimeWidth));
	
	int h;
	if(p.arrival.size() != 3)
	{
		h = 0;
	}
	else
	{
		int a0 = p.arrival[0] - 65;
		int a1 = p.arrival[1] - 65;
		int a2 = p.arrival[2] - 65;
		h = a2 * 29 * 29 + a1 * 29 + a0;
	}
	double r = ((double)(h % 251)) / 251.0;
	double g = ((double)((h * h) % 251)) / 251.0;
	double b = ((double)((h * h * h) % 251)) / 251.0;
	
	
	/*
	int r=0, g=0, b=0;
	if(p.arrival=="HND")
	{

	}
	else
	{
		r = ((double)p.altitude)/40000.0;
	}
	*/
	glColor4d(r, g, b, alpha * 0.75);
	glVertex3d(p.longitude, p.latitude, p.altitude);
	glColor4d(r, g, b, alpha * 0.1);
	glVertex3d(p.longitude, p.latitude, 0.0);
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
	this->runMakePathsBuffer(*bufferPaths, TimeSeparation::Position::next);
	//メインスレッドで直近で使用するトラックデータを取得
	this->makePathsBuffer(*currentPaths, TimeSeparation::Position::current);
	
	initVAO(*currentPaths, vao);
	
	//TrackDataManager tdm;
	//tdm.getTrackDataFromDBParallel(paths,time_min,time_max);
}

void PathsVAO::initVAO(const std::vector<Path>& path, vaoType& v)
{
	std::vector<float> position;
	std::vector<float> color;
	std::vector<unsigned int> element;

	unsigned int imax = path.size();
	for(unsigned int i = 0; i < imax; i++)
	{
		unsigned int jmax =  path[i].pathPoint.size();
		if(imax == 0)
		{
			continue;
		}
		{
			//ダミーの点(同じ点を2つ打つ)
			auto pfirst = path[i].pathPoint.begin();
			//下の点
			position.push_back(pfirst->longitude);
			position.push_back(pfirst->latitude);
			position.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			element.push_back(element.size());
			position.push_back(pfirst->longitude);
			position.push_back(pfirst->latitude);
			position.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			element.push_back(element.size());
			
		}
		for(unsigned int j = 0; j < jmax; j++)
		{
			const PathPoint& pij = path[i].pathPoint[j];
			
			//上の点
			position.push_back(pij.longitude);
			position.push_back(pij.latitude);
			position.push_back(static_cast<float>(pij.altitude));
			float c=((double)pij.altitude)/40000.0f;
			c=c*c;
			color.push_back(   c);
			color.push_back(0.5f);
			color.push_back(1.0f);
			element.push_back(element.size());
			//下の点
			position.push_back(pij.longitude);
			position.push_back(pij.latitude);
			position.push_back(0.9375f * static_cast<float>(pij.altitude));//0.0f);
			color.push_back(   c);
			color.push_back(0.5f);
			color.push_back(1.0f);
			element.push_back(element.size());
			
		}
		{
			//ダミーの点(同じ点を2つ打つ)
			auto plast = path[i].pathPoint.rbegin();
			//下の点
			position.push_back(plast->longitude);
			position.push_back(plast->latitude);
			position.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			element.push_back(element.size());
			position.push_back(plast->longitude);
			position.push_back(plast->latitude);
			position.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			element.push_back(element.size());
		}
	}
	vao.init(position, color, element, GL_TRIANGLE_STRIP);
}


void PathsVAO::resetTime(void)
{
	for(auto&& p: *currentPaths)
	{
		p.past_time_index = 0;
		p.now_index = 0;
	}
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

void PathsVAO::updateNowIndex(Path& p, time_t now)
{
	int path_size = p.pathPoint.size();
	
	//now_indexを更新する
	//now_index[n] = Min{ i | now < paths[n][i].time}
	for(int i = p.now_index; i < path_size; i++)
	{
		if(now < p.pathPoint[i].time)
		{
			p.now_index = i;
			break;
		}
	}
	//nowが軌道のどの時刻よりも大きい場合はpath_sizeを設定する
	if(p.pathPoint[path_size-1].time <= now)
	{
		p.now_index = path_size;
	}
}

void PathsVAO::drawPathLine(Path& p, time_t past_time, time_t now)
{
	int path_size = p.pathPoint.size();
	//past_pointを描画する
	glBegin(GL_TRIANGLE_STRIP);
	//past_time_indexがpの両端のときは対象外の時刻なので描画しない
	if((p.past_time_index != 0) && (p.past_time_index != path_size))
	{
		int i = p.past_time_index;
		PathPoint past_point = getNowPoint(p.pathPoint[i-1], p.pathPoint[i], past_time);
		drawPath(past_point,now);
	}
	//past_time_index <= i < now_indexとなるiのpath[n][i]を描画する
	//path_sizeに達したら描画を終了する
	for(int i = p.past_time_index; i < p.now_index; i++)
	{
		//軌道を描画する
		drawPath(p.pathPoint[i],now);
	}
	//now_indexがpaths[n]の両端のときは対象外の時刻なので描画しない
	//now_pointを描画する
	if((p.now_index != 0) && (p.now_index != path_size))
	{
		int i = p.now_index;
		PathPoint now_point = getNowPoint(p.pathPoint[i-1],p.pathPoint[i],now);
		drawPath(now_point,now);
	}
	glEnd();
}

int PathsVAO::display(time_t now)
{
	
	vao.display();
	
	return 0;
	
	if(timeSeparation.inNextInterval(now))
	{
		//別スレッドでのデータ取得の完了を待つ
		futureMakeBuffer.wait();
		cout << "futureMakeBuffer.wait() finish." << endl;
		
		cout << "(*currentPaths).size() = " << (*currentPaths).size() << endl;
		cout << " (*bufferPaths).size() = " <<  (*bufferPaths).size() << endl;
		
		//バッファの入れ替え
		vector<Path>* dummy = bufferPaths;
		bufferPaths = currentPaths;
		currentPaths = dummy;
		
		//now_indexとpast_time_indexの初期化
		this->resetTime();
		//区間を1つ進める
		timeSeparation.setNextInterval();
		
		//別スレッドで次の区間のトラックデータ取得開始
		this->runMakePathsBuffer(*bufferPaths, TimeSeparation::Position::next);
		
	}
	
	
	//航空機の軌道っぽいものを描く
	//隠面消去を無効にする
	//アルファブレンドで奥の透明オブジェクトが描画されないことがあるため
	//また、軌道の手前に描かれるオブジェクトがないため最後に描画すれば前後関係は崩れない
	glDisable(GL_DEPTH_TEST);
	//アルファブレンド有効化
	glEnable(GL_BLEND);
	//アルファブレンドの方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//描画を始める時刻
	time_t past_time = now - drawTimeWidth;
	
	//描画した航空機数
	int ret = 0;
	
	//航空機毎に軌道を描画する
	for (auto&& p: *currentPaths)
	{
		
		//past_timeより過去の軌道は描かない
		if (p.pathPoint.rbegin()->time < past_time )
		{
			continue;
		}
		
		//未来の軌道は描かない
		if (p.pathPoint[0].time > now )
		{
			continue;
		}
		
		//past_time_indexを更新する
		this->updatePastTimeIndex(p, past_time);
		
		//now_indexを更新する
		this->updateNowIndex(p, now);
		
		//航空機の軌道に垂線をおろした帯状の図形を描画する
		this->drawPathLine(p, past_time, now);
		
		//描画した機数をカウント
		ret++;
	}
	//アルファブレンド無効化
	glDisable(GL_BLEND);
	//デプスバッファ有効化
	glEnable(GL_DEPTH_TEST);
	//航空機の軌道の描画完了
	
	return ret;
}
