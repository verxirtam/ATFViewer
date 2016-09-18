
#include "PathsVAO_cuda_kernel.cuh"


#define VCOUNT (9)
#define VCOUNT2 (18)

#define INDEXCOUNT (4)
#define INDEX_BEGIN (0)
#define INDEX_PAST  (1)
#define INDEX_NOW   (2)
#define INDEX_ELEMENTBEGIN (3)


//インターリーブ配列としてのインデックスと
//頂点配列としてのインデックスを区別するため、
//接尾辞を下記にする
//_index  : 頂点配列としてのインデックス
//_iindex : インターリーブ配列としてのインデックス


__host__
__device__
bool PathsVAO_isInInterval
	(
		float time,
		const float* const vertex_d,
		unsigned int begin_iindex,
		unsigned int end_iindex,
		unsigned int  vertex_iindex
	)
{
	const unsigned int it = 3;
	if((vertex_iindex < begin_iindex) || (end_iindex <= vertex_iindex))
	{
		return false;
	}
	if((vertex_d[vertex_iindex + it] <= time) && (time < vertex_d[vertex_iindex + VCOUNT2 + it]))
	{
		return true;
	}
	return false;
}

__host__
__device__
void PathsVAO_updateTimeVertex
	(
	 float time,
	 const float* const vertex_d,
	 unsigned int begin_iindex,
	 unsigned int end_iindex,
	 unsigned int v_iindex,
	 float* const time_vertex
	)
{
	//各座標を示すインデックス
	const unsigned int ix = 0;
	const unsigned int iy = 1;
	const unsigned int iz = 2;
	const unsigned int it = 3;
	const unsigned int ir = 4;
	const unsigned int ig = 5;
	const unsigned int ib = 6;
	const unsigned int ia = 7;
	
	unsigned int from_iindex = (v_iindex < begin_iindex) ? begin_iindex : v_iindex;
	unsigned int   to_iindex = (v_iindex < begin_iindex) ? begin_iindex : (v_iindex + VCOUNT2);
	from_iindex = (end_iindex <= from_iindex) ? (end_iindex - VCOUNT2) : from_iindex;
	  to_iindex = (end_iindex <=   to_iindex) ? (end_iindex - VCOUNT2) :   to_iindex;
	
	const float* const from = &vertex_d[from_iindex];
	const float* const to   = &vertex_d[  to_iindex];
	float from_time = from[it];
	float   to_time =   to[it];

	//内分比
	float ratio_from = 1.0f - ((float)(time - from_time))/((float)(to_time - from_time));
	float ratio_to = 1.0f - ratio_from;
	
	//上の点の設定
	time_vertex[ix] = ratio_from * from[ix] + ratio_to * to[ix];
	time_vertex[iy] = ratio_from * from[iy] + ratio_to * to[iy];
	time_vertex[iz] = ratio_from * from[iz] + ratio_to * to[iz];
	time_vertex[it] = time;
	time_vertex[ir] = ratio_from * from[ir] + ratio_to * to[ir];
	time_vertex[ig] = ratio_from * from[ig] + ratio_to * to[ig];
	time_vertex[ib] = ratio_from * from[ib] + ratio_to * to[ib];
	time_vertex[ia] = ratio_from * from[ia] + ratio_to * to[ia];
	
	//下の点の設定
	time_vertex[VCOUNT + ix] = ratio_from * from[VCOUNT + ix] + ratio_to * to[VCOUNT + ix];
	time_vertex[VCOUNT + iy] = ratio_from * from[VCOUNT + iy] + ratio_to * to[VCOUNT + iy];
	time_vertex[VCOUNT + iz] = ratio_from * from[VCOUNT + iz] + ratio_to * to[VCOUNT + iz];
	time_vertex[VCOUNT + it] = time;
	time_vertex[VCOUNT + ir] = ratio_from * from[VCOUNT + ir] + ratio_to * to[VCOUNT + ir];
	time_vertex[VCOUNT + ig] = ratio_from * from[VCOUNT + ig] + ratio_to * to[VCOUNT + ig];
	time_vertex[VCOUNT + ib] = ratio_from * from[VCOUNT + ib] + ratio_to * to[VCOUNT + ib];
	time_vertex[VCOUNT + ia] = ratio_from * from[VCOUNT + ia] + ratio_to * to[VCOUNT + ia];
	
	
}



__host__
__device__
void PathsVAO_updateTimeIndex
	(
		float time,
		const float* const vertex_d,
		unsigned int begin_index,
		unsigned int end_index,
		unsigned int* const time_index,
		float* const time_vertex
	)
{
	//インターリーブ配列としてのインデックスと
	//頂点配列としてのインデックスを区別するため、
	//接尾辞を下記にする
	//_index  : 頂点配列としてのインデックス
	//_iindex : インターリーブ配列としてのインデックス
	
	//区間に含まれているかをチェックするインデックス
	unsigned int v_iindex = VCOUNT * (*time_index);
	
	unsigned int begin_iindex = VCOUNT * begin_index;
	unsigned int   end_iindex = VCOUNT *   end_index;
	
	
	//最初の区間の手前の場合
	if(v_iindex < begin_iindex)
	{
		//最初の区間の時刻より手前であれば最初の区間の1つ手前のインデックスを返す
		const unsigned int it = 3;
		//最初の区間の時刻
		float time_first = vertex_d[begin_iindex + it];
		if(time < time_first)
		{
			*time_index = (begin_iindex - VCOUNT2) / VCOUNT;
			return;
		}
	}
	
	//最後の区間の先の場合はそのまま終了
	if(v_iindex >= end_iindex)
	{
		return;
	}
	
	
	//time_indexから順に区間に含まれているかチェックし、
	//含まれている区間の情報から頂点を設定する
	for(; v_iindex < end_iindex; v_iindex+=VCOUNT2)
	{
		if(PathsVAO_isInInterval(time, vertex_d, begin_iindex, end_iindex, v_iindex))
		{
			*time_index = v_iindex / VCOUNT;
			PathsVAO_updateTimeVertex(time, vertex_d, begin_iindex, end_iindex, v_iindex, time_vertex);
			return;
		}
	}
	//見つからなかった場合は最後のインデックスを設定する
	*time_index = end_iindex / VCOUNT;
}

//elementを更新する
__host__
__device__
void PathsVAO_updateElement
	(
		const unsigned int* const index_list_d,
		unsigned int path_index,
		unsigned int* const element_d
	)
{
	unsigned int begin_index = index_list_d[INDEXCOUNT * path_index + INDEX_BEGIN       ];
	unsigned int  past_index = index_list_d[INDEXCOUNT * path_index + INDEX_PAST        ];
	unsigned int   now_index = index_list_d[INDEXCOUNT * path_index + INDEX_NOW         ];
	unsigned int element_begin_index = index_list_d[INDEXCOUNT * path_index + INDEX_ELEMENTBEGIN];
	
	unsigned int begin_index_main = begin_index + 4;
	
	unsigned int end_index         = index_list_d[INDEXCOUNT * (path_index + 1) + INDEX_BEGIN];
	unsigned int element_end_index = index_list_d[INDEXCOUNT * (path_index + 1) + INDEX_ELEMENTBEGIN];
	
	unsigned int past_vertex_index = begin_index;
	unsigned int  now_vertex_index = begin_index + 2;
	
	
	//pastとnowの位置関係を示す真偽値
	//pastは頂点配列の手前にある
	bool past_before = (past_index <  begin_index_main);
	//pastは頂点配列の後にある
	bool past_after  = (end_index  <= past_index );
	//nowは頂点配列の手前にある
	bool now_before  = (now_index  <  begin_index_main);
	//nowは頂点配列の後にある
	bool now_after   = (end_index  <= now_index  );
	
	//pastを描画するかどうか
	bool past_draw = !(past_before || past_after);
	//nowを描画するかどうか
	bool now_draw  = !(now_before  || now_after);
	
	//pastとnowの間の頂点配列の開始・終了のインデックス
	unsigned int main_begin_vertex_index = past_before ? begin_index_main : (past_after ? end_index : (past_index + 2));
	unsigned int   main_end_vertex_index = now_before  ? begin_index_main : (now_after  ? end_index : (now_index  + 2));
	
	//インデックス配列のカウンタ
	unsigned int ei = element_begin_index;
	
	//(最初に2つ設定するダミーのインデックス)
	// =(最初に描画する点のインデックス)を決定する
	unsigned int first_vertex_index = past_draw ? past_vertex_index : main_begin_vertex_index;
	
	//ダミーのインデックスを設定する
	element_d[ei] = first_vertex_index; ei++;
	element_d[ei] = first_vertex_index; ei++;
	//描画する頂点のインデックスを設定する
	if(past_draw)
	{
		element_d[ei] = past_vertex_index    ; ei++;
		element_d[ei] = past_vertex_index + 1; ei++;
	}
	unsigned int sei = main_begin_vertex_index;
	for(;(ei < element_end_index) && (sei < main_end_vertex_index); ei++, sei++)
	{
		element_d[ei] = sei;
	}
	if(now_draw)
	{
		element_d[ei] = now_vertex_index    ; ei++;
		element_d[ei] = now_vertex_index + 1; ei++;
	}
	//最後に描画した頂点のインデックスをダミーとして最後まで設定する
	unsigned int last_vertex_index = element_d[ei -1];
	for(;ei < element_end_index; ei++)
	{
		element_d[ei] = last_vertex_index;
	}


	/*
	element_d[ei] = past_vertex_index; ei++;
	element_d[ei] = past_vertex_index; ei++;
	
	element_d[ei] = past_vertex_index;     ei++;
	element_d[ei] = past_vertex_index + 1; ei++;
	
	unsigned int sei = past_index + 2;
	for(; (ei < element_end_index) && (sei < now_index + 2); ei++, sei++)
	{		element_d[ei] = sei;
	}
	if(ei < end_index)
	{
		element_d[ei] = now_vertex_index;
		ei++;
	}
	for(;ei < element_end_index;ei++)
	{
		element_d[ei] = now_vertex_index + 1;
	}
	*/
}

/**
 * @brief nowに応じてVAOを更新する
 *
 * block 1つにpath 1つが対応する
 *
 * @param now
 * @param vertex_d
 * @param element_d
 * @param index_list_d
 * @param path_count
 *
 * @return 
 */
__host__
__device__
void PathsVAO_updateDeviceDataCUDAMain
	(
		float now,
		float past,
		float* const vertex_d,
		unsigned int* const element_d,
		unsigned int* const index_list_d,
		unsigned int path_index
	)
{
	//path[path_index]に関連するインデックス
	unsigned int begin_index        =  index_list_d[INDEXCOUNT * path_index + INDEX_BEGIN];
	unsigned int* const past_index  = &index_list_d[INDEXCOUNT * path_index + INDEX_PAST ];
	unsigned int* const now_index   = &index_list_d[INDEXCOUNT * path_index + INDEX_NOW  ];
	
	//最後の次のインデックス
	unsigned int end_index = index_list_d[INDEXCOUNT * (path_index + 1) + INDEX_BEGIN];
	
	//path[path_index]の頂点数
	//unsigned int vertex_count = end_index - begin_index;
	
	//pastの頂点
	float* const past_vertex = &vertex_d[VCOUNT * begin_index];
	
	//nowの頂点
	float* const now_vertex = &vertex_d[VCOUNT * begin_index + VCOUNT2];
	
	//pastIndexを更新する
	PathsVAO_updateTimeIndex(past, vertex_d, begin_index + 4, end_index, past_index, past_vertex);
	
	//nowIndexを更新する
	PathsVAO_updateTimeIndex( now, vertex_d, begin_index + 4, end_index,  now_index,  now_vertex);
	
	//elementを更新する
	PathsVAO_updateElement(index_list_d, path_index, element_d);
}


__global__
void PathsVAO_updateDeviceDataCUDA
	(
		float now,
		float past,
		float* const vertex_d,
		unsigned int* const element_d,
		unsigned int* const index_list_d
	)
{
	//pathのインデックス
	unsigned int path_index = blockIdx.x;
	//メインの処理に渡す
	PathsVAO_updateDeviceDataCUDAMain(now, past, vertex_d, element_d, index_list_d, path_index);
}

