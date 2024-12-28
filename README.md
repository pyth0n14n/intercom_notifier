# intercom_notifier
インターホンが鳴ったらLineに通知する

## モチベーション
2Fにいると、インターホンが鳴っても音に気付かないことがある。
インターホンが鳴ったらLineに通知が行くようにする。

## 準備
必要なもの
- ESP32モジュール (ESP-WROOM-32 DevKitなど)
- ワイヤ (A接点 to EPS32) x2
- インターホン ()

ハードウェア設定
- ドアホンのA接点からワイヤを2本接続しておく (結線作業時はブレーカーを落とすこと)
- QiコネクタなどでESP32側を保護しておくと良い

Lineアクセストークン (Messaging API)
- line notifier がサービス終了予定 (2025/03) のため、Messaging APIを利用する
  - 1000通/月 フリープラン のため、用途的にこれで十分
- LineビジネスIDを発行
- 通知用アカウントを作成
- プロバイダーを作成して紐づける（このあたり失念）

## プログラム
- main.hppを作成し、SSID/PASSWORD, LINE_ACCESS_TOKENを #define で設定。
  - NTPモジュールを別途DLする必要がある
- ビルド→インストール

- A接点から生やしたワイヤを、ESP GPIO27とGNDにそれぞれ接続する
  - インターホンOFF: A接点はオープンのため、PULL-upされて HIGH の状態
  - インターホンON: A接点はショートのため、GNDに接続されて LOW の状態

## 参考文献

1. @yomori, "ドアホンが鳴ったらGoogleHome,Slack,Discordに通知する (ESP32)", Qiita, 2023,
https://qiita.com/yomori/items/ca213f1087c2a0e270e1
2. @nak435, "【ESP32・MicroPython】ドアホンが鳴ったらLINEに通知する", Qiita, 2024,
https://qiita.com/nak435/items/879b647aa3d7343ba0be
3. 電人さん, "ネット非対応のテレビドアホンを対応させる(但し通知のみ)", 2023, https://king.mineo.jp/reports/220936