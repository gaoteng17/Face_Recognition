package com.henu.swface.activity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.ContactsContract;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.StaggeredGridLayoutManager;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.henu.swface.Adapter.ManageAdapter;
import com.henu.swface.Database.DatabaseAdapter;
import com.henu.swface.R;
import com.henu.swface.Utils.FinalUtil;
import com.henu.swface.VO.UserHasSigned;

import java.util.ArrayList;
import java.util.List;

import cn.bmob.v3.Bmob;
import cn.bmob.v3.BmobQuery;
import cn.bmob.v3.exception.BmobException;
import cn.bmob.v3.listener.FindListener;
import in.srain.cube.views.ptr.PtrClassicFrameLayout;
import in.srain.cube.views.ptr.PtrFrameLayout;
import in.srain.cube.views.ptr.PtrHandler;
import in.srain.cube.views.ptr.PtrUIHandler;
import in.srain.cube.views.ptr.indicator.PtrIndicator;

public class ManageActivity extends Activity {

	private RecyclerView recyclerView_manage;
	//    private Dialog dialog;
	private Toolbar toolbar;
	private PtrClassicFrameLayout ptrFrame;
	private ArrayList<UserHasSigned> mList;
	private ManageAdapter adapter;
	private static final String TAG = ManageActivity.class.getSimpleName();
	private static final int ManageActivity_Request = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_manage);
		recyclerView_manage = (RecyclerView) findViewById(R.id.recyclerView_manage);
		ptrFrame = (PtrClassicFrameLayout) findViewById(R.id.fragment_rotate_header_with_view_group_frame);
		initPtrFrame();
		toolbar = (Toolbar) findViewById(R.id.toolbar_manage);
		toolbar.setNavigationIcon(R.mipmap.button_back);
		toolbar.setNavigationOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				finish();
			}
		});
		toolbar.setTitle("已注册用户");
		toolbar.inflateMenu(R.menu.base_toolbar_menu);
		StaggeredGridLayoutManager layoutManager = new StaggeredGridLayoutManager(3, StaggeredGridLayoutManager.VERTICAL);
		recyclerView_manage.setLayoutManager(layoutManager);
		mList = new ArrayList<>();
		adapter = new ManageAdapter(mList);
		recyclerView_manage.setAdapter(adapter);
	}

	private void initPtrFrame() {
		ptrFrame.setPtrHandler(new PtrHandler() {
			@Override
			public void onRefreshBegin(PtrFrameLayout frame) {
				new InitDate().execute();
			}

			@Override
			public boolean checkCanDoRefresh(PtrFrameLayout frame, View content, View header) {
				Log.i(TAG, "checkCanDoRefresh: "+recyclerView_manage.getScaleY()+" "+recyclerView_manage.getScaleX());
				//判断当前是否在recyclerView_manage的顶部
				if(recyclerView_manage.getChildAt(0).getY()==0f){
					return true;
				}else{
					return false;
				}

			}


		});
		ptrFrame.setLastUpdateTimeRelateObject(this);
		// the following are default settings
		ptrFrame.setResistance(1.7f);
		ptrFrame.setRatioOfHeaderHeightToRefresh(1.2f);
		ptrFrame.setDurationToClose(200);
		ptrFrame.setDurationToCloseHeader(1800);
		// default is false
		ptrFrame.setPullToRefresh(false);
		// default is true
		ptrFrame.setKeepHeaderWhenRefresh(true);

		// scroll then refresh
		// comment in base fragment
		ptrFrame.postDelayed(new Runnable() {
			@Override
			public void run() {
				// ptrFrame.autoRefresh();
				ptrFrame.autoRefresh(true);
			}
		}, 150);
	}

	@Override
	protected void onResume() {
		super.onResume();
		//initData();

	}


	private class InitDate extends AsyncTask<Integer, Void, Void> {
		@Override
		protected Void doInBackground(final Integer... integers) {
			Log.i(TAG, "doInBackground: " + integers);
			DatabaseAdapter db = new DatabaseAdapter(ManageActivity.this);
			mList.clear();
			mList.addAll(db.findAllUser_User());
			db = null;
			adapter.setOnItemClickListener(new ManageAdapter.OnItemClickListener() {
				@Override
				public void onItemClick(View view, int position) {
					Intent intent = new Intent(ManageActivity.this, FaceDetailActivity.class);
					intent.putExtra("userHasSigned", mList.get(position));
					intent.putExtra("position", position);
					startActivityForResult(intent, ManageActivity_Request);
				}
			});

			return null;
		}

		@Override
		protected void onPostExecute(Void aVoid) {
			super.onPostExecute(aVoid);
			adapter.notifyDataSetChanged();
			findBmobSql();
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.i(TAG, "onActivityResult: resultCode" + resultCode);
		if (resultCode == 0) {
			if (data != null) {
				UserHasSigned userHasSigned = (UserHasSigned) data.getSerializableExtra("userHasSigned");
				int position = data.getIntExtra("position", -1);
				if (position > -1) {
					UserHasSigned userHasSigned1 = mList.get(position);
					userHasSigned1.setFace_token1(userHasSigned.getFace_token1());
					userHasSigned1.setFace_token2(userHasSigned.getFace_token2());
					userHasSigned1.setFace_token3(userHasSigned.getFace_token3());
					userHasSigned1.setFace_token4(userHasSigned.getFace_token4());
					userHasSigned1.setFace_token5(userHasSigned.getFace_token5());
					userHasSigned1.setFace_url1(userHasSigned.getFace_url1());
					userHasSigned1.setFace_url2(userHasSigned.getFace_url2());
					userHasSigned1.setFace_url3(userHasSigned.getFace_url3());
					userHasSigned1.setFace_url4(userHasSigned.getFace_url4());
					userHasSigned1.setFace_url5(userHasSigned.getFace_url5());
					userHasSigned1.setUser_name(userHasSigned.getUser_name());
					adapter.notifyItemChanged(position);
					adapter.notifyDataSetChanged();
				}
			}
		} else {
			if (data != null) {
				int position = data.getIntExtra("position", -1);
				if(position>-1){
					mList.remove(position);
					Log.i(TAG, "onActivityResult_mList.remove: ");
					adapter.notifyItemRemoved(position);
					adapter.notifyItemRangeChanged(position,mList.size());
				}
			}
		}
	}

	private void findBmobSql() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				Log.i(TAG, "run: " + Thread.currentThread().getId());
				BmobQuery<UserHasSigned> query = new BmobQuery<>();
				SharedPreferences sharedPreferences = getSharedPreferences("login", Context.MODE_PRIVATE);
				String telephone = sharedPreferences.getString("username", "");
				query.addWhereEqualTo("telephone", telephone);
				query.setLimit(50);
				query.findObjects(new FindListener<UserHasSigned>() {
					@Override
					public void done(final List<UserHasSigned> list, final BmobException e) {
						new Thread(new Runnable() {
							@Override
							public void run() {
								if (e == null) {
									DatabaseAdapter db = new DatabaseAdapter(ManageActivity.this);
									for (UserHasSigned userHasSigned : list) {
										if (!mList.contains(userHasSigned)) {
											Log.i(TAG, "mList.contains(userHasSigned): false");
											db.addUser_User(userHasSigned, null);
											mList.add(userHasSigned);
										}
									}
									for (UserHasSigned userHasSigned : mList) {
										Log.i(TAG, "done: " + userHasSigned.toString() + Thread.currentThread().getId());
									}
									Message message = Message.obtain();
									message.arg1 = FinalUtil.SYN_DATA_SUCCESS;
									myHandler.sendMessage(message);
								} else {
									Log.e(TAG, "query.findObjects: ", e);
									Message message = Message.obtain();
									message.arg1 = FinalUtil.SYN_DATA_FAILED;
									myHandler.sendMessage(message);
								}
							}
						}).start();
					}
				});
			}
		}).start();

	}

	private Handler myHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.arg1) {
				case FinalUtil.SYN_DATA_SUCCESS:
					notifyData();
					break;
				case FinalUtil.SYN_DATA_FAILED:
					Toast.makeText(getApplicationContext(), "查询出错，请检查", Toast.LENGTH_LONG).show();
					ptrFrame.refreshComplete();
					break;
				default:
					break;
			}
		}
	};

	private void notifyData() {
		synchronized (adapter) {
			//adapter.notifyAll();
			adapter.notifyDataSetChanged();
			Log.i(TAG, "adapter.notifyAll()_online:success " + Thread.currentThread().getId());
			ptrFrame.refreshComplete();
		}
	}

}
