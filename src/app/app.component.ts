import { Component, HostListener, OnDestroy, OnInit } from '@angular/core';
import { IMqttMessage, MqttConnectionState, MqttService } from 'ngx-mqtt';
import { ESPData, SeriesData } from './data';
import { NotificationService } from './services/notification.service';

@Component({
    selector: 'app-root',
    templateUrl: './app.component.html',
    styleUrl: './app.component.css'
})
export class AppComponent implements OnInit, OnDestroy {
    on = false;
    current = new Date();
    granted = false;
    innerWidth: number;
    data: ESPData[] = [];
    lastUpdate = { temp: 0, humi: 0, lum: null };
    public setting = '';
    legend: boolean = false;
    showLabels: boolean = false;
    animations = false;
    xAxis: boolean = true;
    yAxis: boolean = true;
    timeline: boolean = true;


    constructor(private MQTT: MqttService, private notif: NotificationService) {
        this.innerWidth = window.innerWidth - 20;
        Object.assign(this, { ESPData, SeriesData });
    }

    ngOnInit() {
        setInterval(() => this.current = new Date(), 1000);
        this.setDataStructure();
        this.MQTT.state.subscribe((state) => this.on = state === MqttConnectionState.CONNECTED);
        this.MQTT.observe('arduino/network').subscribe((msg: IMqttMessage) => {
            const json = JSON.parse(msg.payload.toString());
            this.addMesures(json);
            this.lastUpdate.temp = json.temp;
            this.lastUpdate.humi = Math.floor(json.humi);
            this.lastUpdate.lum = json.lux;
            this.updateSetting(json.lux);
            this.data = [...this.data];
            if (json.temp > 45) this.notif.sendNotification(json.temp);
        });

    }


    @HostListener('window:resize', ['$event'])
    onResize(event: any) {
        this.innerWidth = event.target.innerWidth;
    }

    setDataStructure() {
        // clear data
        this.data = [];
        this.data.push(new ESPData('Temperature'));
        this.data.push(new ESPData('Humidity'));
        this.data.push(new ESPData('Luminosity'));
    }


    addMesures(json: any) {
        this.data.forEach((espData, i) => {
            espData.series.push(new SeriesData(json, i));
            if (espData.series.length >= 15)
                espData.series.shift();
        });
    }

    test() {
        this.granted = this.notif.requestPermission();
    }

    updateSetting(lux: number) {
        if (lux < 20) this.setting = 'Dark room';
        else if (lux <= 100) this.setting = 'Residential Area or Corridor';
        else if (lux <= 300) this.setting = 'General Office Work';
        else if (lux <= 500) this.setting = 'Detailed Workspace';
        else this.setting = 'Technical Facility or Laboratory';
    }


    ngOnDestroy(): void {
        this.MQTT.disconnect();
    }
}
