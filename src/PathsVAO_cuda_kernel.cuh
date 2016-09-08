

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
	);

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
	);


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
	);

//elementを更新する
__host__
__device__
void PathsVAO_updateElement
	(
		const unsigned int* const index_list_d,
		unsigned int path_index,
		unsigned int* const element_d
	);

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
__global__
void PathsVAO_updateDeviceDataCUDA
	(
		float now,
		float past,
		float* const vertex_d,
		unsigned int* const element_d,
		unsigned int* const index_list_d,
		unsigned int path_count
	);




