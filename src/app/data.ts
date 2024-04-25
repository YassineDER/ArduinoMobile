export class SeriesData {
    value: number;
    name: string;

    constructor(json: any, index: number){
        const currentTime = new Date();
        const hours = currentTime.getHours().toString().padStart(2, '0');
        const minutes = currentTime.getMinutes().toString().padStart(2, '0');
        const seconds = currentTime.getSeconds().toString().padStart(2, '0');

        switch(index){
            case 0:
                this.value = json.temp;
                break;
            case 1:
                this.value = json.humi;
                break;
            case 2:
                this.value = json.lux;
                break;
            default:
                this.value = 0;
        }

        this.name = `${hours}:${minutes}:${seconds}`;
    }
}

export class ESPData {
    name: string;
    series: SeriesData[];

    constructor(name: string){
        this.name = name;
        this.series = [];
    }

}

  