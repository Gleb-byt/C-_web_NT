# uvicorn mock_ml_api:app --port 8000 --reload
from fastapi import FastAPI
import random
from string import ascii_lowercase as alph
from pydantic import BaseModel

random.seed(42)

app = FastAPI()


def tr(original: str) -> str:  # type: ignore
    text_res: str = "".join(random.choice(alph) for _ in range(random.randint(10, 100)))
    return text_res


def ev(translation: str, original: str):
    return random.random()


class TranslateReq(BaseModel):

    text: str


class EvaluateReq(BaseModel):
    user_translation: str
    reference: str


@app.post("/translate")
def translate(req: TranslateReq):
    print(f"Request on translation of sentense: {req.text}")

    return {"translated_text": tr(req.text)}


@app.post("/evaluate")
def evaluate(req: EvaluateReq):
    print(f"Evaluate: user='{req.user_translation}', ref='{req.reference}'")
    return {
        "score": ev(req.user_translation, req.reference),
        "feedback": "NT bro",
        "error": "",
        "success": True,
    }
