// Состояние приложения (state)
const state = {
  currentRound: null, // { original: string, reference: string }
  history: [], // массив завершенных раундов
};

// Получаем элементы DOM
const btnStart = document.getElementById("btn-start");
const btnSubmit = document.getElementById("btn-submit");
const statusIndicator = document.getElementById("status-indicator");

const taskSection = document.getElementById("task-section");
const originalTextEl = document.getElementById("original-text");
const userInputEl = document.getElementById("user-translation-input");

const resultsSection = document.getElementById("results-section");
const scoreBadge = document.getElementById("score-badge");
const resUserText = document.getElementById("res-user-text");
const resAiText = document.getElementById("res-ai-text");
const resFeedbackText = document.getElementById("res-feedback-text");
const historyList = document.getElementById("history-list");

// Утилита для изменения статуса
function setStatus(text, isLoading = false) {
  statusIndicator.textContent = text;
  if (isLoading) {
    statusIndicator.className = "status-indicator status-loading";
  } else {
    statusIndicator.className = "status-indicator status-idle";
  }
}

// ─── 1. Старт нового раунда (POST /start) ─────────────────────────
btnStart.addEventListener("click", async () => {
  try {
    setStatus("Получаем фразу от сервера...", true);
    btnStart.disabled = true;

    // Скрываем предыдущие результаты, если они были
    resultsSection.classList.add("hidden");
    userInputEl.value = "";

    const response = await fetch("/start", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
    });

    if (!response.ok) {
      throw new Error(`Ошибка сервера: ${response.status}`);
    }

    const data = await response.json();
    console.log("Получены данные раунда:", data);

    // Сохраняем состояние текущего раунда
    state.currentRound = {
      original: data.original,
      reference: data.reference,
    };

    // Отображаем интерфейс ввода
    originalTextEl.textContent = data.original;
    taskSection.classList.remove("hidden");
    userInputEl.focus();

    setStatus("Раунд начался!");
  } catch (error) {
    console.error("Ошибка при старте раунда:", error);
    alert("Не удалось начать раунд. Проверьте консоль браузера.");
    setStatus("Ошибка соединения");
  } finally {
    btnStart.disabled = false;
  }
});

// ─── 2. Отправка перевода на оценку (POST /evaluate) ──────────────
btnSubmit.addEventListener("click", async () => {
  const userText = userInputEl.value.trim();

  if (!userText) {
    alert("Пожалуйста, введите ваш вариант перевода!");
    userInputEl.focus();
    return;
  }

  if (!state.currentRound) {
    alert("Сначала нажмите «Начать раунд»!");
    return;
  }

  try {
    setStatus("Нейросеть оценивает перевод...", true);
    btnSubmit.disabled = true;

    // Формируем payload для отправки в C++ сервер
    const payload = {
      user_translation: userText,
      reference: state.currentRound.reference,
      original: state.currentRound.original,
    };

    const response = await fetch("/evaluate", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });

    if (!response.ok) {
      throw new Error(`Ошибка сервера: ${response.status}`);
    }

    const result = await response.json();
    console.log("Результат оценки:", result);

    // Отображаем результаты
    scoreBadge.textContent = `Оценка: ${(result.score * 100).toFixed(0)}%`;
    resUserText.textContent = userText;
    resAiText.textContent = state.currentRound.reference;
    resFeedbackText.textContent = result.feedback || "Перевод проверен.";

    resultsSection.classList.remove("hidden");
    setStatus("Проверка завершена");

    // Сохраняем в историю и перерисовываем
    saveToHistory({
      original: state.currentRound.original,
      userTranslation: userText,
      score: result.score,
    });
  } catch (error) {
    console.error("Ошибка при оценке перевода:", error);
    alert("Не удалось отправить перевод на проверку.");
    setStatus("Ошибка оценки");
  } finally {
    btnSubmit.disabled = false;
  }
});

// ─── 3. Сохранение и отображение истории ─────────────────────────
function saveToHistory(item) {
  state.history.unshift(item); // добавляем в начало

  // Очищаем заглушку "пусто", если есть
  historyList.innerHTML = "";

  state.history.forEach((round, index) => {
    const el = document.createElement("div");
    el.className = "history-item";
    el.innerHTML = `
      <div class="history-item-details">
        <strong>${round.original}</strong>
        <span style="color: #94a3b8;">${round.userTranslation}</span>
      </div>
      <div class="history-score">${(round.score * 100).toFixed(0)}%</div>
    `;
    historyList.appendChild(el);
  });
}
