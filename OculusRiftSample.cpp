// ウィンドウ関連の処理
#include "GgApplication.h"

// 視点
constexpr GLfloat zNear(0.1f);                          // 前方面までの距離
constexpr GLfloat zFar(5.0f);                           // 後方面までの距離
constexpr GLfloat origin[] = { 0.0f, 0.0f, -3.0f };     // 基準位置

// 光源
constexpr GgSimpleShader::Light light =
{
  { 0.1f, 0.1f, 0.1f, 1.0f },                           // 環境光成分
  { 1.0f, 1.0f, 1.0f, 1.0f },                           // 拡散反射光成分
  { 1.0f, 1.0f, 1.0f, 1.0f },                           // 鏡面光成分
  { 0.0f, 1.0f, 1.0f, 0.0f }                            // 位置
};

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("Oculus Rift Sample", 1280, 720);

  // プログラムオブジェクトの作成
  const GgSimpleShader simple("simple.vert", "simple.frag");

  // 光源
  const GgSimpleShader::LightBuffer lightBuffer(light);

  // 図形
  const GgSimpleObj object("box.obj");

  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // 隠面消去を有効にする
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // 焦点距離 (cameraNear) が 1 のときのスクリーンの大きさ
    GLfloat screen[4] = { -window.getAspect(), window.getAspect(), -1.0f, 1.0f };

    // 投影変換行列を求める
    const GgMatrix mp(ggFrustum(screen[0] * zNear, screen[1] * zNear,
      screen[2] * zNear, screen[3] * zNear, zNear, zFar));

    // 視野変換行列を求める
    const GgMatrix mv(ggTranslate(origin[0], origin[1], origin[2]));

    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 光源をワールド座標系に配置する
    lightBuffer.loadPosition(mv * light.position);

    // シェーダプログラムの使用開始
    simple.use(mp, mv * window.getTrackball(), lightBuffer);

    // 図形を描画する
    object.draw();

    // カラーバッファを入れ替える
    window.swapBuffers();
  }
}
