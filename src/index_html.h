// src/index_html.h
#pragma once
#include <string>

const std::string INDEX_HTML = R"rawhtml(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Premium URL Shortener</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700;800&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-base: #060913;
            --card-bg: rgba(13, 18, 33, 0.45);
            --card-border: rgba(255, 255, 255, 0.07);
            --primary: #818cf8;
            --primary-dark: #6366f1;
            --primary-glow: rgba(99, 102, 241, 0.25);
            --accent: #ec4899;
            --accent-glow: rgba(236, 72, 153, 0.2);
            --text-main: #f3f4f6;
            --text-muted: #9ca3af;
            --success: #34d399;
            --success-glow: rgba(52, 211, 153, 0.15);
            --error: #f87171;
            --error-glow: rgba(248, 113, 113, 0.15);
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            font-family: 'Outfit', sans-serif;
        }

        body {
            background-color: var(--bg-base);
            color: var(--text-main);
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            overflow-x: hidden;
            position: relative;
            padding: 2rem 1.5rem;
        }

        /* Animated Mesh Background Blobs */
        .ambient-bg {
            position: absolute;
            width: 100vw;
            height: 100vh;
            top: 0;
            left: 0;
            z-index: -1;
            overflow: hidden;
            pointer-events: none;
        }

        .blob {
            position: absolute;
            border-radius: 50%;
            filter: blur(140px);
            opacity: 0.25;
            animation: float 25s infinite alternate ease-in-out;
        }

        .blob-1 {
            width: 500px;
            height: 500px;
            background: radial-gradient(circle, var(--primary) 0%, #4f46e5 100%);
            top: -100px;
            right: -100px;
        }

        .blob-2 {
            width: 600px;
            height: 600px;
            background: radial-gradient(circle, var(--accent) 0%, #be185d 100%);
            bottom: -150px;
            left: -150px;
            animation-duration: 30s;
        }

        @keyframes float {
            0% { transform: translate(0, 0) scale(1); }
            50% { transform: translate(60px, 80px) scale(1.15); }
            100% { transform: translate(-40px, -60px) scale(0.9); }
        }

        /* Container styling */
        .container {
            width: 100%;
            max-width: 920px;
            z-index: 1;
            display: flex;
            flex-direction: column;
            gap: 1.75rem;
        }

        /* Brand Title */
        header {
            text-align: center;
            margin-bottom: 1rem;
            animation: fadeInDown 0.8s cubic-bezier(0.16, 1, 0.3, 1);
        }

        header h1 {
            font-size: 4rem;
            font-weight: 800;
            letter-spacing: -0.05em;
            margin-bottom: 0.5rem;
            background: linear-gradient(135deg, #ffffff 40%, #c7d2fe 70%, #fbcfe8 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            text-shadow: 0 4px 20px rgba(0, 0, 0, 0.35);
        }

        header p {
            font-size: 1.2rem;
            color: var(--text-muted);
            font-weight: 300;
        }

        /* Glassmorphic Container */
        .card {
            background: var(--card-bg);
            border: 1px solid var(--card-border);
            border-radius: 28px;
            padding: 3rem 3.5rem;
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
            backdrop-filter: blur(25px);
            -webkit-backdrop-filter: blur(25px);
            position: relative;
            overflow: hidden;
            animation: fadeInUp 0.8s cubic-bezier(0.16, 1, 0.3, 1);
        }

        .card::after {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            height: 1px;
            background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.15), transparent);
        }

        /* Tab Navigation */
        .tabs {
            display: flex;
            background: rgba(15, 23, 42, 0.4);
            border: 1px solid var(--card-border);
            border-radius: 14px;
            padding: 0.35rem;
            margin-bottom: 2rem;
            position: relative;
        }

        .tab-btn {
            flex: 1;
            background: none;
            border: none;
            color: var(--text-muted);
            font-size: 0.95rem;
            font-weight: 600;
            padding: 0.75rem 0;
            cursor: pointer;
            border-radius: 10px;
            transition: all 0.25s ease;
            outline: none;
        }

        .tab-btn:hover {
            color: #fff;
        }

        .tab-btn.active {
            color: #fff;
            background: rgba(255, 255, 255, 0.08);
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
        }

        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
            animation: fadeIn 0.4s ease-out;
        }

        /* Inputs */
        .form-group {
            margin-bottom: 1.5rem;
        }

        .form-group label {
            display: block;
            font-size: 0.8rem;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.06em;
            color: var(--text-muted);
            margin-bottom: 0.5rem;
        }

        .input-field {
            width: 100%;
            background: rgba(10, 15, 30, 0.5);
            border: 1px solid rgba(255, 255, 255, 0.07);
            border-radius: 14px;
            padding: 1.25rem 1.5rem;
            font-size: 1.05rem;
            color: #fff;
            outline: none;
            transition: all 0.25s cubic-bezier(0.16, 1, 0.3, 1);
        }

        .input-field:focus {
            border-color: var(--primary);
            box-shadow: 0 0 0 4px var(--primary-glow);
            background: rgba(10, 15, 30, 0.75);
        }

        .form-row {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 1.25rem;
        }

        @media (max-width: 480px) {
            .form-row {
                grid-template-columns: 1fr;
            }
        }

        /* Buttons */
        .btn {
            width: 100%;
            background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
            border: none;
            border-radius: 14px;
            padding: 1.15rem;
            font-size: 1rem;
            font-weight: 600;
            color: #fff;
            cursor: pointer;
            transition: all 0.25s cubic-bezier(0.16, 1, 0.3, 1);
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 0.5rem;
            margin-top: 1rem;
            box-shadow: 0 10px 20px -5px var(--primary-glow);
        }

        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 15px 25px -5px var(--primary-glow);
            filter: brightness(1.1);
        }

        .btn:active {
            transform: translateY(0);
        }

        /* Beautiful Result Section */
        .result-container {
            margin-top: 2rem;
            background: linear-gradient(135deg, rgba(52, 211, 153, 0.05) 0%, rgba(52, 211, 153, 0.01) 100%);
            border: 1px solid rgba(52, 211, 153, 0.18);
            border-radius: 20px;
            padding: 1.5rem;
            display: none;
            animation: popIn 0.4s cubic-bezier(0.16, 1, 0.3, 1);
        }

        .result-container.error {
            background: linear-gradient(135deg, rgba(248, 113, 113, 0.05) 0%, rgba(248, 113, 113, 0.01) 100%);
            border-color: rgba(248, 113, 113, 0.18);
        }

        .result-header {
            display: flex;
            align-items: center;
            gap: 0.5rem;
            font-size: 0.85rem;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.05em;
            color: var(--success);
            margin-bottom: 0.75rem;
        }

        .result-container.error .result-header {
            color: var(--error);
        }

        .result-box {
            display: flex;
            gap: 0.75rem;
        }

        .result-url {
            flex-grow: 1;
            background: rgba(10, 15, 30, 0.6);
            border: 1px solid rgba(255, 255, 255, 0.05);
            border-radius: 12px;
            padding: 0.9rem 1.1rem;
            font-size: 1.05rem;
            font-weight: 500;
            color: #fff;
            word-break: break-all;
            display: flex;
            align-items: center;
        }

        .copy-btn {
            background: rgba(255, 255, 255, 0.07);
            border: 1px solid rgba(255, 255, 255, 0.05);
            border-radius: 12px;
            color: #fff;
            padding: 0 1.5rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.2s ease;
            white-space: nowrap;
        }

        .copy-btn:hover {
            background: rgba(255, 255, 255, 0.15);
            border-color: rgba(255, 255, 255, 0.1);
        }

        .visit-btn {
            background: rgba(99, 102, 241, 0.15);
            border: 1px solid rgba(99, 102, 241, 0.3);
            border-radius: 12px;
            color: var(--primary);
            padding: 0 1.2rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.2s ease;
            white-space: nowrap;
            text-decoration: none;
            display: flex;
            align-items: center;
            gap: 0.3rem;
        }

        .visit-btn:hover {
            background: rgba(99, 102, 241, 0.25);
            border-color: rgba(99, 102, 241, 0.5);
            color: #fff;
        }

        /* Deduplication info notice */
        .info-notice {
            font-size: 0.78rem;
            color: var(--text-muted);
            margin-top: 0.75rem;
            line-height: 1.4;
            display: flex;
            align-items: center;
            gap: 0.35rem;
        }

        /* Analytics Results View */
        .analytics-view {
            margin-top: 2rem;
            display: none;
            animation: popIn 0.4s cubic-bezier(0.16, 1, 0.3, 1);
        }

        .analytics-title {
            font-size: 1.1rem;
            font-weight: 700;
            margin-bottom: 1rem;
            padding-bottom: 0.5rem;
            border-bottom: 1px solid var(--card-border);
            color: #fff;
        }

        .analytics-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 1.25rem;
        }

        @media (max-width: 480px) {
            .analytics-grid {
                grid-template-columns: 1fr;
            }
        }

        .analytics-card {
            background: rgba(10, 15, 30, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.04);
            border-radius: 16px;
            padding: 1.25rem;
            display: flex;
            flex-direction: column;
            gap: 0.25rem;
        }

        .analytics-card.full-width {
            grid-column: 1 / -1;
        }

        .analytics-label {
            font-size: 0.75rem;
            text-transform: uppercase;
            letter-spacing: 0.06em;
            color: var(--text-muted);
        }

        .analytics-value {
            font-size: 1.25rem;
            font-weight: 700;
            color: #fff;
            word-break: break-all;
        }

        .analytics-value.accented {
            background: linear-gradient(135deg, var(--accent) 0%, #db2777 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        /* Stats Panel */
        .cache-panel {
            background: var(--card-bg);
            border: 1px solid var(--card-border);
            border-radius: 20px;
            padding: 1.5rem 2rem;
            display: flex;
            justify-content: space-around;
            text-align: center;
            backdrop-filter: blur(25px);
            -webkit-backdrop-filter: blur(25px);
            animation: fadeInUp 0.8s cubic-bezier(0.16, 1, 0.3, 1);
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
        }

        .cache-item {
            flex: 1;
            padding: 0.25rem 0;
        }

        .cache-item:not(:last-child) {
            border-right: 1px solid var(--card-border);
        }

        .cache-label {
            font-size: 0.7rem;
            text-transform: uppercase;
            letter-spacing: 0.08em;
            color: var(--text-muted);
            margin-bottom: 0.4rem;
        }

        .cache-value {
            font-size: 1.4rem;
            font-weight: 800;
            color: #fff;
        }

        .cache-value.glow-green {
            color: var(--success);
            text-shadow: 0 0 10px rgba(52, 211, 153, 0.3);
        }

        .cache-value.glow-purple {
            color: var(--primary);
            text-shadow: 0 0 10px rgba(129, 140, 248, 0.3);
        }

        /* Animations */
        @keyframes fadeInDown {
            from { opacity: 0; transform: translateY(-25px); }
            to { opacity: 1; transform: translateY(0); }
        }

        @keyframes fadeInUp {
            from { opacity: 0; transform: translateY(25px); }
            to { opacity: 1; transform: translateY(0); }
        }

        @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
        }

        @keyframes popIn {
            from { opacity: 0; transform: scale(0.96) translateY(12px); }
            to { opacity: 1; transform: scale(1) translateY(0); }
        }
    </style>
</head>
<body>
    <div class="ambient-bg">
        <div class="blob blob-1"></div>
        <div class="blob blob-2"></div>
    </div>

    <div class="container">
        <header>
            <h1>URL-shortener</h1>
            <p>High performance, in-memory cached link shortening</p>
        </header>

        <div class="card">
            <div class="tabs">
                <button class="tab-btn active" id="btn-shorten" onclick="switchTab('shorten')">Shorten URL</button>
                <button class="tab-btn" id="btn-analytics" onclick="switchTab('analytics')">URL Analytics</button>
            </div>

            <!-- Shorten Tab -->
            <div id="tab-shorten" class="tab-content active">
                <form id="shorten-form" onsubmit="shortenURL(event)">
                    <div class="form-group">
                        <label for="long-url">Destination URL</label>
                        <input type="url" id="long-url" class="input-field" placeholder="https://example.com/very-long-and-complex-url-path" required>
                    </div>

                    <div class="form-row">
                        <div class="form-group">
                            <label for="custom-alias">Custom Alias (Optional)</label>
                            <input type="text" id="custom-alias" class="input-field" placeholder="e.g., promo2026">
                        </div>
                        <div class="form-group">
                            <label for="expiry-days">Expires In (Days, Optional)</label>
                            <input type="number" id="expiry-days" class="input-field" min="1" placeholder="e.g., 7">
                        </div>
                    </div>

                    <button type="submit" class="btn">Shorten Now</button>
                </form>

                <!-- Success Result -->
                <div id="shorten-result" class="result-container">
                    <div class="result-header">
                        <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">
                            <polyline points="20 6 9 17 4 12"></polyline>
                        </svg>
                        Short Link Created
                    </div>
                    <div class="result-box">
                        <div id="result-url-text" class="result-url"></div>
                        <button class="copy-btn" id="btn-copy" onclick="copyResult()">Copy</button>
                        <a class="visit-btn" id="btn-visit" href="#" target="_blank" rel="noopener" title="Opens in new tab — stats will update on the dashboard">
                            Visit ↗
                        </a>
                    </div>
                    <div class="info-notice">
                        <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                            <circle cx="12" cy="12" r="10"></circle>
                            <line x1="12" y1="16" x2="12" y2="12"></line>
                            <line x1="12" y1="8" x2="12.01" y2="8"></line>
                        </svg>
                        Use "Visit" to open in a new tab so stats update live. 1st visit = Miss, repeat = Hit.
                    </div>
                </div>

                <!-- Error Result -->
                <div id="shorten-error" class="result-container error">
                    <div class="result-header">Error</div>
                    <div id="shorten-error-text"></div>
                </div>
            </div>

            <!-- Analytics Tab -->
            <div id="tab-analytics" class="tab-content">
                <form id="analytics-form" onsubmit="queryAnalytics(event)">
                    <div class="form-group">
                        <label for="short-code-input">Enter Short Code</label>
                        <input type="text" id="short-code-input" class="input-field" placeholder="e.g., promo2026" required>
                    </div>
                    <button type="submit" class="btn">View Analytics</button>
                </form>

                <div id="analytics-error" class="result-container error" style="margin-top:2.5rem;">
                    <div class="result-header">Error</div>
                    <div id="analytics-error-text"></div>
                </div>

                <!-- Analytics Display Grid -->
                <div id="analytics-result" class="analytics-view">
                    <div class="analytics-title">Redirect Statistics</div>
                    <div class="analytics-grid">
                        <div class="analytics-card">
                            <span class="analytics-label">Short Code</span>
                            <span id="stat-code" class="analytics-value"></span>
                        </div>
                        <div class="analytics-card">
                            <span class="analytics-label">Clicks / Hits</span>
                            <span id="stat-hits" class="analytics-value accented">0</span>
                        </div>
                        <div class="analytics-card full-width">
                            <span class="analytics-label">Destination URL</span>
                            <span id="stat-original" class="analytics-value" style="font-size:0.95rem;font-weight:500;"></span>
                        </div>
                        <div class="analytics-card">
                            <span class="analytics-label">Created At</span>
                            <span id="stat-created" class="analytics-value" style="font-size:0.9rem;"></span>
                        </div>
                        <div class="analytics-card">
                            <span class="analytics-label">Code Type</span>
                            <span id="stat-type" class="analytics-value" style="font-size:0.9rem;"></span>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Live Stats footer -->
        <div class="cache-panel">
            <div class="cache-item">
                <div class="cache-label">Total URLs</div>
                <div id="stat-total-urls" class="cache-value glow-purple">0</div>
            </div>
            <div class="cache-item">
                <div class="cache-label">Total Hits</div>
                <div id="stat-total-hits" class="cache-value glow-purple">0</div>
            </div>
            <div class="cache-item">
                <div class="cache-label">Cache Hits</div>
                <div id="cache-hits" class="cache-value">0</div>
            </div>
            <div class="cache-item">
                <div class="cache-label">Cache Misses</div>
                <div id="cache-misses" class="cache-value">0</div>
            </div>
            <div class="cache-item">
                <div class="cache-label">Cache Hit Rate</div>
                <div id="cache-hit-rate" class="cache-value glow-green">—</div>
            </div>
        </div>
    </div>

    <script>
        function switchTab(tabId) {
            document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
            document.querySelectorAll('.tab-content').forEach(content => content.classList.remove('active'));

            if (tabId === 'shorten') {
                document.getElementById('btn-shorten').classList.add('active');
                document.getElementById('tab-shorten').classList.add('active');
            } else {
                document.getElementById('btn-analytics').classList.add('active');
                document.getElementById('tab-analytics').classList.add('active');
            }
        }

        let currentAnalyticsCode = '';

        async function shortenURL(e) {
            e.preventDefault();
            const longUrl = document.getElementById('long-url').value;
            const customAlias = document.getElementById('custom-alias').value.trim();
            const expiryDays = document.getElementById('expiry-days').value;

            document.getElementById('shorten-result').style.display = 'none';
            document.getElementById('shorten-error').style.display = 'none';

            const payload = { long_url: longUrl };
            if (customAlias) payload.custom_alias = customAlias;
            if (expiryDays) payload.expires_in_days = parseInt(expiryDays);

            try {
                const response = await fetch('/shorten', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(payload)
                });

                const data = await response.json();
                if (!response.ok) throw new Error(data.error || 'Failed to shorten URL');

                document.getElementById('result-url-text').textContent = data.short_url;
                document.getElementById('shorten-result').style.display = 'block';
                // Set the Visit button href so it opens in a new tab
                document.getElementById('btn-visit').href = data.short_url;
                updateStats();
            } catch (err) {
                document.getElementById('shorten-error-text').textContent = err.message;
                document.getElementById('shorten-error').style.display = 'block';
            }
        }

        async function queryAnalytics(e) {
            e.preventDefault();
            const code = document.getElementById('short-code-input').value.trim();
            currentAnalyticsCode = code;

            document.getElementById('analytics-result').style.display = 'none';
            document.getElementById('analytics-error').style.display = 'none';

            try {
                const response = await fetch(`/analytics/${code}`);
                const data = await response.json();

                if (!response.ok) throw new Error(data.error || 'Short code not found');

                document.getElementById('stat-code').textContent = data.short_code;
                document.getElementById('stat-hits').textContent = data.hit_count;
                document.getElementById('stat-original').textContent = data.original_url;

                const date = new Date(data.created_at * 1000);
                document.getElementById('stat-created').textContent = date.toLocaleString();
                document.getElementById('stat-type').textContent = data.is_custom_alias ? 'Custom Alias' : 'Auto-Generated';

                document.getElementById('analytics-result').style.display = 'block';
            } catch (err) {
                document.getElementById('analytics-error-text').textContent = err.message;
                document.getElementById('analytics-error').style.display = 'block';
            }
        }

        function copyResult() {
            const text = document.getElementById('result-url-text').textContent;
            
            // Check if navigator.clipboard is available (requires HTTPS or localhost in many browsers)
            if (navigator.clipboard && window.isSecureContext) {
                navigator.clipboard.writeText(text).then(() => {
                    showCopySuccess();
                }).catch(err => {
                    fallbackCopy(text);
                });
            } else {
                fallbackCopy(text);
            }
        }

        function fallbackCopy(text) {
            const textArea = document.createElement("textarea");
            textArea.value = text;
            textArea.style.position = "fixed"; // Avoid scrolling to bottom
            textArea.style.top = "0";
            textArea.style.left = "0";
            textArea.style.opacity = "0";
            document.body.appendChild(textArea);
            textArea.focus();
            textArea.select();
            try {
                const successful = document.execCommand('copy');
                if (successful) {
                    showCopySuccess();
                } else {
                    console.error('Fallback copy command was unsuccessful');
                }
            } catch (err) {
                console.error('Fallback copy error:', err);
            }
            document.body.removeChild(textArea);
        }

        function showCopySuccess() {
            const btn = document.getElementById('btn-copy');
            btn.textContent = 'Copied!';
            btn.style.background = 'var(--success)';
            btn.style.borderColor = 'rgba(52, 211, 153, 0.4)';
            setTimeout(() => {
                btn.textContent = 'Copy';
                btn.style.background = '';
                btn.style.borderColor = '';
            }, 2000);
        }

        async function updateStats() {
            try {
                const res = await fetch('/stats');
                if (res.ok) {
                    const s = await res.json();
                    document.getElementById('stat-total-urls').textContent = s.total_urls;
                    document.getElementById('stat-total-hits').textContent = s.total_hits;
                    document.getElementById('cache-hits').textContent = s.cache_hits;
                    document.getElementById('cache-misses').textContent = s.cache_misses;
                    const total = s.cache_hits + s.cache_misses;
                    document.getElementById('cache-hit-rate').textContent =
                        total > 0 ? parseFloat(s.hit_rate).toFixed(1) + '%' : '—';
                }
            } catch (e) {
                console.error(e);
            }
            // Auto-refresh analytics hit count when that tab is active
            if (currentAnalyticsCode &&
                document.getElementById('tab-analytics').classList.contains('active') &&
                document.getElementById('analytics-result').style.display !== 'none') {
                try {
                    const ar = await fetch(`/analytics/${currentAnalyticsCode}`);
                    if (ar.ok) {
                        const ad = await ar.json();
                        document.getElementById('stat-hits').textContent = ad.hit_count;
                    }
                } catch (_) {}
            }
        }

        updateStats();
        setInterval(updateStats, 3000);
    </script>
</body>
</html>
)rawhtml";
