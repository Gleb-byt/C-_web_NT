# uvicorn mock_ml_api:app --port 8000 --reload
from fastapi import FastAPI
import random
from string import ascii_lowercase as alph
from pydantic import BaseModel

random.seed(42)

app = FastAPI()


class TranslateReq:

    def __call__(original: str):
        text_res: str = "".join(
            random.choice(alph) for _ in range(random.randint(10, 100))
        )

        return text_res


class EvaluateReq:

    def __call__(translation: str) -> float:
        return random.random()


@app.post("/translate")
def translate(req: TranslateReq):
    print("Request on translation of ")
