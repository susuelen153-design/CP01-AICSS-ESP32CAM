#pragma once

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="pt-BR">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32-CAM Dataset Capture</title>
  <style>
    :root {
      --bg: #eef1f5;
      --bg-grid: rgba(17, 24, 39, .035);
      --surface: #ffffff;
      --surface-soft: #f8fafc;
      --ink: #111827;
      --muted: #6b7280;
      --line: #d9dee7;
      --line-dark: #29313d;
      --dark: #0b0f14;
      --dark-2: #111821;
      --dark-3: #16202b;
      --blue: #2563eb;
      --blue-dark: #1d4ed8;
      --amber: #f59e0b;
      --green: #16a34a;
      --red: #dc2626;
      --red-soft: #fef2f2;
      --radius: 18px;
      --shadow: 0 22px 60px rgba(15, 23, 42, .14);
      --mono: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace;
      --sans: Inter, ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", Arial, sans-serif;
    }

    * { box-sizing: border-box; }

    body {
      margin: 0;
      min-height: 100vh;
      color: var(--ink);
      font-family: var(--sans);
      background:
        linear-gradient(transparent 0 31px, var(--bg-grid) 31px 32px),
        linear-gradient(90deg, transparent 0 31px, var(--bg-grid) 31px 32px),
        radial-gradient(circle at top left, rgba(37, 99, 235, .13), transparent 34%),
        var(--bg);
      background-size: 32px 32px, 32px 32px, auto, auto;
    }

    main {
      width: min(1180px, calc(100% - 32px));
      margin: 0 auto;
      padding: 28px 0 44px;
    }

    .page-header {
      display: flex;
      align-items: flex-end;
      justify-content: space-between;
      gap: 18px;
      margin-bottom: 20px;
    }

    .brand {
      display: flex;
      align-items: center;
      gap: 14px;
    }

    .device-light {
      width: 13px;
      height: 13px;
      border-radius: 999px;
      background: var(--green);
      box-shadow: 0 0 0 6px rgba(22, 163, 74, .12), 0 0 18px rgba(22, 163, 74, .75);
      flex: 0 0 auto;
    }

    .eyebrow {
      margin: 0 0 6px;
      color: var(--blue);
      font: 700 12px/1 var(--mono);
      letter-spacing: .18em;
      text-transform: uppercase;
    }

    h1 {
      margin: 0;
      font-size: clamp(26px, 4vw, 42px);
      line-height: 1;
      letter-spacing: -1.5px;
    }

    .subtitle {
      margin: 10px 0 0;
      color: var(--muted);
      font-size: 15px;
      max-width: 620px;
    }

    .device-tag {
      min-width: 210px;
      padding: 12px 14px;
      border: 1px solid var(--line);
      border-radius: 14px;
      background: rgba(255, 255, 255, .72);
      color: var(--muted);
      text-align: right;
      font: 600 11px/1.7 var(--mono);
      letter-spacing: .08em;
      text-transform: uppercase;
      box-shadow: 0 10px 28px rgba(15, 23, 42, .06);
    }

    .device-tag b {
      display: block;
      color: var(--ink);
      font-size: 12px;
    }

    .capture-layout {
      display: grid;
      grid-template-columns: minmax(0, 1.55fr) minmax(300px, .9fr);
      gap: 18px;
      align-items: stretch;
    }

    .panel {
      background: var(--surface);
      border: 1px solid var(--line);
      border-radius: var(--radius);
      box-shadow: var(--shadow);
    }

    .stream-panel {
      padding: 14px;
      background: linear-gradient(180deg, #111821 0%, #0b0f14 100%);
      border-color: var(--line-dark);
    }

    .viewer-topbar {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 12px;
      padding: 0 4px 12px;
      color: #94a3b8;
      font: 700 11px/1 var(--mono);
      letter-spacing: .14em;
      text-transform: uppercase;
    }

    .viewer-topbar span:first-child {
      color: #e5e7eb;
    }

    .stream-frame {
      position: relative;
      aspect-ratio: 4 / 3;
      overflow: hidden;
      border-radius: 14px;
      background: #05070a;
      border: 1px solid #2b3442;
      box-shadow:
        inset 0 0 70px rgba(0, 0, 0, .58),
        0 24px 60px rgba(0, 0, 0, .38);
    }

    #stream {
      width: 100%;
      height: 100%;
      object-fit: contain;
      display: block;
      background: #05070a;
    }

    .stream-frame::after {
      content: "";
      position: absolute;
      inset: 0;
      pointer-events: none;
      background: linear-gradient(rgba(255,255,255,.045), transparent 18%);
      background-size: 100% 7px;
      mix-blend-mode: overlay;
      opacity: .22;
    }

    .corner {
      position: absolute;
      width: 28px;
      height: 28px;
      border: 2px solid var(--amber);
      pointer-events: none;
      opacity: .92;
    }
    .corner.tl { top: 14px; left: 14px; border-right: 0; border-bottom: 0; }
    .corner.tr { top: 14px; right: 14px; border-left: 0; border-bottom: 0; }
    .corner.bl { bottom: 14px; left: 14px; border-right: 0; border-top: 0; }
    .corner.br { bottom: 14px; right: 14px; border-left: 0; border-top: 0; }

    .live-tag,
    .rec-tag {
      position: absolute;
      z-index: 2;
      display: inline-flex;
      align-items: center;
      gap: 8px;
      border-radius: 999px;
      background: rgba(5, 7, 10, .72);
      border: 1px solid rgba(255, 255, 255, .12);
      backdrop-filter: blur(6px);
      color: #cbd5e1;
      font: 800 11px/1 var(--mono);
      letter-spacing: .16em;
      text-transform: uppercase;
      padding: 8px 10px;
    }

    .live-tag {
      left: 16px;
      bottom: 16px;
    }

    .live-tag::before {
      content: "";
      width: 8px;
      height: 8px;
      border-radius: 999px;
      background: var(--green);
      box-shadow: 0 0 12px rgba(22, 163, 74, .8);
    }

    .rec-tag {
      right: 16px;
      top: 16px;
      color: #fecaca;
      opacity: 0;
      transform: translateY(-4px);
      transition: .18s ease;
    }

    .rec-tag::before {
      content: "";
      width: 9px;
      height: 9px;
      border-radius: 999px;
      background: var(--red);
    }

    body.is-collecting .rec-tag {
      opacity: 1;
      transform: translateY(0);
    }

    body.is-collecting .rec-tag::before {
      animation: blink 1s steps(1) infinite;
    }

    body.is-collecting .stream-frame {
      border-color: rgba(220, 38, 38, .9);
      box-shadow:
        inset 0 0 80px rgba(220, 38, 38, .08),
        0 0 0 1px rgba(220, 38, 38, .34),
        0 24px 60px rgba(0, 0, 0, .38);
    }

    @keyframes blink { 50% { opacity: .15; } }

    .control-panel {
      padding: 22px;
      display: flex;
      flex-direction: column;
      gap: 18px;
    }

    .panel-title {
      margin: 0;
      font-size: 20px;
      letter-spacing: -.02em;
    }

    .panel-description {
      margin: -10px 0 0;
      color: var(--muted);
      font-size: 14px;
      line-height: 1.45;
    }

    .toolbar {
      display: grid;
      gap: 14px;
    }

    label {
      display: grid;
      gap: 7px;
      color: var(--muted);
      font: 800 11px/1 var(--mono);
      letter-spacing: .12em;
      text-transform: uppercase;
    }

    input {
      width: 100%;
      min-height: 46px;
      border: 1px solid var(--line);
      border-radius: 12px;
      padding: 0 13px;
      background: var(--surface-soft);
      color: var(--ink);
      font: 600 15px/1 var(--sans);
      transition: border-color .15s, box-shadow .15s, background .15s;
    }

    input::placeholder { color: #9ca3af; }
    input:focus {
      outline: none;
      border-color: var(--blue);
      background: #fff;
      box-shadow: 0 0 0 4px rgba(37, 99, 235, .12);
    }
    input:disabled { opacity: .62; cursor: not-allowed; }

    .button-row {
      display: grid;
      grid-template-columns: 1fr;
      gap: 10px;
    }

    button {
      min-height: 46px;
      border: 1px solid transparent;
      border-radius: 12px;
      padding: 0 15px;
      cursor: pointer;
      font: 800 12px/1 var(--mono);
      letter-spacing: .1em;
      text-transform: uppercase;
      transition: transform .15s ease, box-shadow .15s ease, border-color .15s ease, background .15s ease;
    }

    button:hover { transform: translateY(-1px); }
    button:active { transform: translateY(0); }

    #collect {
      background: var(--blue);
      color: #fff;
      box-shadow: 0 12px 28px rgba(37, 99, 235, .24);
    }

    #collect:hover { background: var(--blue-dark); }

    #collect.collecting {
      background: var(--red);
      box-shadow: 0 12px 28px rgba(220, 38, 38, .24);
    }

    #clear {
      color: var(--red);
      background: var(--red-soft);
      border-color: #fecaca;
    }

    #download {
      color: #92400e;
      background: #fffbeb;
      border-color: #fde68a;
    }

    .stats {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
      gap: 10px;
      margin-top: auto;
    }

    .stat-card {
      border: 1px solid var(--line);
      border-radius: 14px;
      background: var(--surface-soft);
      padding: 13px;
    }

    .stat-label {
      display: block;
      margin-bottom: 8px;
      color: var(--muted);
      font: 800 10px/1 var(--mono);
      letter-spacing: .12em;
      text-transform: uppercase;
    }

    #status,
    #modeStatus {
      display: block;
      color: var(--ink);
      font-size: 21px;
      font-weight: 850;
      letter-spacing: -.02em;
    }

    #modeStatus {
      color: var(--green);
      font-size: 17px;
    }

    body.is-collecting #modeStatus { color: var(--red); }

    .hint {
      margin: 0;
      padding: 13px 14px;
      border-left: 4px solid var(--blue);
      border-radius: 12px;
      background: #eff6ff;
      color: #334155;
      font-size: 13px;
      line-height: 1.45;
    }

    .gallery-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 14px;
      margin: 30px 0 14px;
    }

    .gallery-title {
      margin: 0;
      color: var(--ink);
      font-size: 18px;
    }

    .gallery-line {
      flex: 1;
      height: 1px;
      background: var(--line);
    }

    .gallery-note {
      color: var(--muted);
      font: 700 11px/1 var(--mono);
      letter-spacing: .08em;
      text-transform: uppercase;
    }

    #grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(158px, 1fr));
      gap: 12px;
      counter-reset: shot;
    }

    .thumb {
      position: relative;
      overflow: hidden;
      aspect-ratio: 4 / 3;
      border-radius: 14px;
      background: var(--dark);
      border: 1px solid var(--line);
      box-shadow: 0 10px 24px rgba(15, 23, 42, .08);
    }

    .thumb img {
      width: 100%;
      height: 100%;
      display: block;
      object-fit: cover;
    }

    .thumb::before {
      counter-increment: shot;
      content: counter(shot, decimal-leading-zero);
      position: absolute;
      left: 8px;
      bottom: 8px;
      z-index: 1;
      padding: 4px 7px;
      border-radius: 999px;
      background: rgba(5, 7, 10, .72);
      color: #fbbf24;
      font: 800 10px/1 var(--mono);
      letter-spacing: .08em;
    }

    .thumb button {
      position: absolute;
      top: 8px;
      right: 8px;
      width: 30px;
      height: 30px;
      min-height: 0;
      padding: 0;
      display: grid;
      place-items: center;
      border-radius: 999px;
      background: rgba(5, 7, 10, .72);
      color: #fecaca;
      border: 1px solid rgba(255, 255, 255, .12);
      font: 900 15px/1 var(--sans);
      opacity: 0;
      transform: none;
    }

    .thumb:hover button,
    .thumb:focus-within button { opacity: 1; }
    .thumb button:hover { background: var(--red); color: #fff; transform: none; }

    @media (max-width: 900px) {
      .capture-layout { grid-template-columns: 1fr; }
      .control-panel { order: 2; }
    }

    @media (max-width: 640px) {
      main { width: min(100% - 22px, 1180px); padding-top: 18px; }
      .page-header { align-items: flex-start; flex-direction: column; }
      .device-tag { width: 100%; text-align: left; }
      .stats { grid-template-columns: 1fr; }
      .gallery-header { align-items: flex-start; flex-direction: column; }
      .gallery-line { width: 100%; flex: 0 0 1px; }
    }
  </style>
</head>
<body>
  <main>
    <header class="page-header">
      <div class="brand">
        <span class="device-light" aria-hidden="true"></span>
        <div>
          <p class="eyebrow">ESP32-CAM</p>
          <h1>Captura de Imagens</h1>
          <p class="subtitle">Coleta local de imagens para treinamento customizado de modelo YOLO.</p>
        </div>
      </div>
      <div class="device-tag">
        <b>Prof. Dr. Noris</b>
      </div>
    </header>

    <section class="capture-layout" aria-label="Área de captura">
      <div class="panel stream-panel">
        <div class="viewer-topbar">
          <span>Live Preview</span>
          <span>stream</span>
        </div>
        <div class="stream-frame">
          <img id="stream" src="/stream" alt="Stream ao vivo da ESP32-CAM">
          <span class="corner tl"></span>
          <span class="corner tr"></span>
          <span class="corner bl"></span>
          <span class="corner br"></span>
          <span class="live-tag">Online</span>
          <span class="rec-tag">REC</span>
        </div>
      </div>

      <aside class="panel control-panel" aria-label="Controles da coleta">
        <h2 class="panel-title">Configuração da coleta</h2>
        <p class="panel-description">Defina o nome base, capture quadros automaticamente e exporte o dataset em ZIP.</p>

        <div class="toolbar">
          <label>
            Nome base dos arquivos
            <input id="baseName" type="text" placeholder="ex: trena" required>
          </label>

          <div class="button-row">
            <button id="collect">Iniciar coleta</button>
            <button id="clear">Limpar imagens</button>
            <button id="download">Baixar ZIP</button>
          </div>
        </div>

        <div class="stats" aria-label="Indicadores da coleta">
          <div class="stat-card">
            <span class="stat-label">Capturadas</span>
            <span id="status">0 imagens</span>
          </div>
          <div class="stat-card">
            <span class="stat-label">Estado</span>
            <span id="modeStatus">Pronto</span>
          </div>
        </div>

        <p class="hint">Mantenha o objeto bem enquadrado antes de iniciar. A coleta salva imagens JPG sequenciais usando o nome base informado.</p>
      </aside>
    </section>

    <section class="gallery-header" aria-label="Cabeçalho da galeria">
      <h2 class="gallery-title">Capturas do dataset</h2>
      <span class="gallery-line" aria-hidden="true"></span>
      <span class="gallery-note">Prévia local</span>
    </section>

    <section id="grid" aria-label="Imagens capturadas"></section>
  </main>

  <script>
    const stream = document.getElementById('stream');
    const grid = document.getElementById('grid');
    const statusEl = document.getElementById('status');
    const modeStatus = document.getElementById('modeStatus');
    const collectBtn = document.getElementById('collect');
    const baseNameInput = document.getElementById('baseName');
    const files = [];
    let collecting = false;
    let timer = null;
    let fileBase = '';

    function setStatus() {
      statusEl.textContent = `${files.length} ${files.length === 1 ? 'imagem' : 'imagens'}`;
    }

    function setCollectingState(active) {
      collecting = active;
      document.body.classList.toggle('is-collecting', active);
      collectBtn.textContent = active ? 'Parar coleta' : 'Iniciar coleta';
      collectBtn.classList.toggle('collecting', active);
      modeStatus.textContent = active ? 'Coletando' : 'Pronto';
    }

    function sanitizeName(text) {
      return text
        .trim()
        .normalize('NFD')
        .replace(/[̀-ͯ]/g, '')
        .replace(/[^a-zA-Z0-9_-]+/g, '_')
        .replace(/^_+|_+$/g, '');
    }

    function dataUrlToBytes(dataUrl) {
      const binary = atob(dataUrl.split(',')[1]);
      const bytes = new Uint8Array(binary.length);
      for (let i = 0; i < binary.length; i++) bytes[i] = binary.charCodeAt(i);
      return bytes;
    }

    function captureFrame() {
      const width = stream.naturalWidth || 640;
      const height = stream.naturalHeight || 480;
      const canvas = document.createElement('canvas');
      canvas.width = width;
      canvas.height = height;
      canvas.getContext('2d').drawImage(stream, 0, 0, width, height);

      const dataUrl = canvas.toDataURL('image/jpeg', 0.92);
      const name = `${fileBase}_${String(files.length + 1).padStart(4, '0')}.jpg`;
      files.push({ name, data: dataUrlToBytes(dataUrl), url: dataUrl });

      const item = document.createElement('div');
      item.className = 'thumb';
      item.innerHTML = `<img src="${dataUrl}" alt="${name}"><button title="Remover" aria-label="Remover ${name}">×</button>`;
      item.querySelector('button').onclick = () => {
        const index = Array.from(grid.children).indexOf(item);
        if (index >= 0) files.splice(index, 1);
        item.remove();
        setStatus();
      };
      grid.appendChild(item);
      setStatus();
    }

    collectBtn.onclick = () => {
      if (!collecting) {
        fileBase = sanitizeName(baseNameInput.value);
        if (!fileBase) {
          alert('Digite um nome base para os arquivos antes de iniciar a coleta.');
          baseNameInput.focus();
          return;
        }
        setCollectingState(true);
        baseNameInput.disabled = true;
        captureFrame();
        timer = setInterval(captureFrame, 700);
      } else {
        setCollectingState(false);
        clearInterval(timer);
      }
    };

    document.getElementById('clear').onclick = () => {
      clearInterval(timer);
      setCollectingState(false);
      baseNameInput.disabled = false;
      fileBase = '';
      files.length = 0;
      grid.innerHTML = '';
      setStatus();
    };

    function crc32(bytes) {
      let crc = -1;
      for (let i = 0; i < bytes.length; i++) {
        crc ^= bytes[i];
        for (let j = 0; j < 8; j++) crc = (crc >>> 1) ^ (0xedb88320 & -(crc & 1));
      }
      return (crc ^ -1) >>> 0;
    }

    function u16(value) {
      return new Uint8Array([value & 255, (value >>> 8) & 255]);
    }

    function u32(value) {
      return new Uint8Array([value & 255, (value >>> 8) & 255, (value >>> 16) & 255, (value >>> 24) & 255]);
    }

    function textBytes(text) {
      return new TextEncoder().encode(text);
    }

    function concat(parts) {
      const size = parts.reduce((sum, part) => sum + part.length, 0);
      const out = new Uint8Array(size);
      let offset = 0;
      for (const part of parts) {
        out.set(part, offset);
        offset += part.length;
      }
      return out;
    }

    function createZip(entries) {
      const locals = [];
      const centrals = [];
      let offset = 0;
      const now = new Date();
      const time = (now.getHours() << 11) | (now.getMinutes() << 5) | Math.floor(now.getSeconds() / 2);
      const date = ((now.getFullYear() - 1980) << 9) | ((now.getMonth() + 1) << 5) | now.getDate();

      for (const entry of entries) {
        const name = textBytes(entry.name);
        const crc = crc32(entry.data);
        const local = concat([
          u32(0x04034b50), u16(20), u16(0), u16(0), u16(time), u16(date),
          u32(crc), u32(entry.data.length), u32(entry.data.length),
          u16(name.length), u16(0), name, entry.data
        ]);
        const central = concat([
          u32(0x02014b50), u16(20), u16(20), u16(0), u16(0), u16(time), u16(date),
          u32(crc), u32(entry.data.length), u32(entry.data.length),
          u16(name.length), u16(0), u16(0), u16(0), u16(0), u32(0), u32(offset), name
        ]);
        locals.push(local);
        centrals.push(central);
        offset += local.length;
      }

      const centralSize = centrals.reduce((sum, part) => sum + part.length, 0);
      const end = concat([
        u32(0x06054b50), u16(0), u16(0), u16(entries.length), u16(entries.length),
        u32(centralSize), u32(offset), u16(0)
      ]);
      return new Blob([...locals, ...centrals, end], { type: 'application/zip' });
    }

    document.getElementById('download').onclick = () => {
      if (!files.length) return;
      const a = document.createElement('a');
      a.href = URL.createObjectURL(createZip(files));
      a.download = `${fileBase || sanitizeName(baseNameInput.value) || 'dataset'}.zip`;
      a.click();
      URL.revokeObjectURL(a.href);
    };
  </script>
</body>
</html>
)rawliteral";
